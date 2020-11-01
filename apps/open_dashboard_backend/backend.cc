		// Copyright (C) 2020 twyleg
#include "backend.h"

#include <open_dashboard_common/config.h>
#include <open_dashboard_common/packet.h>

#include <open_dashboard.pb.h>

#include <google/protobuf/text_format.h>

#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

#include <iostream>
#include <string>

namespace {

constexpr const char* CONFIG_DEFAULT_HOSTNAME = "127.0.0.1";
constexpr int CONFIG_DEFAULT_PORT = 50000;


std::string MessageToString(const google::protobuf::Message& message)
{
	std::string s;
	google::protobuf::TextFormat::PrintToString(message, &s);
	return s;
}

}

CliArguments::CliArguments(int argc, char* argv[])
{
	namespace po = boost::program_options;

	po::variables_map vm;
	po::options_description desc("Available options:");
	desc.add_options()
		("help", "Print help message.")
		("port,p", po::value<int>(&mPort)->default_value(CONFIG_DEFAULT_PORT), "Destination port")
		("hostname,h", po::value<std::string>(&mHostname)->default_value(CONFIG_DEFAULT_HOSTNAME), "Destination hostname")
		("config,c", po::value<std::filesystem::path>(), "Config")
		("qml-file", po::value<std::filesystem::path>(), "QML main file")
		("sidebars-disabled", po::bool_switch(&mSidebarsDisabled), "Disable sidebars")
	;

	po::positional_options_description p;
	p.add("qml-file", -1);

	try {
		po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
		po::notify(vm);
	} catch (const boost::program_options::required_option& e) {
		std::cout << "Missing required argument " << e.get_option_name() << std::endl;
		std::cout << desc << std::endl;
		std::exit(-1);
	}

	if (!vm["config"].empty())
		mConfigFilePath.emplace(vm["config"].as<std::filesystem::path>());

	if (!vm["qml-file"].empty())
		mMainQmlFilePath.emplace(vm["qml-file"].as<std::filesystem::path>());

	if (vm.count("help")) {
		std::cout << desc << std::endl;
		std::exit(0);
	}
}

void CliArguments::ValidateRuntimeArguments()
{
	if (mConfigFilePath)
	{
		THROW_IF(!std::filesystem::is_regular_file(*mConfigFilePath), "Config file does not exist: {}", mConfigFilePath->string());
		THROW_IF(mConfigFilePath->extension() != ".xml", "Config file does not have \".xml\" extension: {}", mConfigFilePath->string());
	}

	if (mMainQmlFilePath)
	{
		THROW_IF(!std::filesystem::is_regular_file(*mMainQmlFilePath), "Qml file does not exist: {}", mMainQmlFilePath->string());
		THROW_IF(mMainQmlFilePath->extension() != ".qml", "Qml file does not have \".qml\" extension: {}", mMainQmlFilePath->string());
	}
}

Backend::Backend(int argc, char* argv[], QObject* parent)
:	QObject(parent),
	mArgc(argc),
	mArgv(argv),
	cliArguments(mArgc, mArgv),
	mApplication(mArgc, mArgv),
	mReceiveTimer(this),
	mUdpReceiver(cliArguments.mHostname, cliArguments.mPort),
	mUdpImuReceiver(cliArguments.mHostname, cliArguments.mPort+1)
{
	cliArguments.ValidateRuntimeArguments();
}

Backend::~Backend()
{
	emit(unloadFrontendRequest());
}

int Backend::Run()
{
	mApplication.setOrganizationName("OpenDashboardBackend");
	mApplication.setOrganizationDomain("OpenDashboardBackend");

	mControlDataStaticModel.SetSidebarsDisabled(cliArguments.mSidebarsDisabled);

	if (cliArguments.mConfigFilePath)
	{
		auto config = OpenDashboard::Common::Config::ReadConfig(*cliArguments.mConfigFilePath);

		mControlDataStaticModel.SetVideoChannelOnePath(QString::fromStdString(
				std::filesystem::absolute(config.mVideoData[0]).generic_string()));
		mControlDataStaticModel.SetVideoChannelTwoPath(QString::fromStdString(
				std::filesystem::absolute(config.mVideoData[1]).generic_string()));
		mControlDataStaticModel.SetVideoChannelThreePath(QString::fromStdString(
				std::filesystem::absolute(config.mVideoData[2]).generic_string()));
	}

	mEngine.addImportPath("qrc:/");
	mEngine.rootContext()->setContextProperty("controlDataStaticModel", &mControlDataStaticModel);
	mEngine.rootContext()->setContextProperty("controlDataDynamicModel", &mControlDataDynamicModel);
	mEngine.rootContext()->setContextProperty("dataModel", &mVehicleDataModel);
	mEngine.rootContext()->setContextProperty("backend", this);
	mEngine.load(QUrl(QStringLiteral("qrc:/qml/Application.qml")));

	if (cliArguments.mMainQmlFilePath)
		LoadFrontend(QUrl::fromLocalFile(QString::fromStdString(cliArguments.mMainQmlFilePath->generic_string())));
	else
		LoadStartscreen();

	QObject::connect(&mReceiveTimer,SIGNAL(timeout()), this, SLOT(handleTimer()));
	mReceiveTimer.start(10);

	return mApplication.exec();
}

void Backend::handleOpenFileRequest(const QUrl& filepath)
{
	LoadFrontend(filepath);
}

void Backend::handleReloadRequest()
{
	ReloadFrontend();
}

void Backend::LoadStartscreen()
{
	const QUrl qApplicationPathUrl("qrc:/qml/LogoStartscreen.qml");

	QVector<QQmlContext::PropertyPair> newProperties = {
		{ QString("qApplication"), qApplicationPathUrl }
	};

	mEngine.rootContext()->setContextProperties(newProperties);
	emit(loadFrontendRequest());
}

void Backend::LoadFrontend(const QUrl& frontendMainFileUrl)
{
	const std::filesystem::path qApplicationPath = frontendMainFileUrl.toLocalFile().toStdString();
	const std::filesystem::path qwd = std::filesystem::absolute(qApplicationPath).parent_path();

	const QUrl qApplicationPathUrl = QUrl::fromLocalFile(QString::fromStdString(std::filesystem::absolute(qApplicationPath).generic_string()));
	const QUrl qwdUrl = QUrl::fromLocalFile(QString::fromStdString(std::filesystem::absolute(qwd).generic_string()));

	QVector<QQmlContext::PropertyPair> newProperties = {
		{ QString("qApplication"), qApplicationPathUrl },
		{ QString("qwd"), qwdUrl }
	};

	emit(unloadFrontendRequest());
	mEngine.rootContext()->setContextProperties(newProperties);
	emit(loadFrontendRequest());
}

void Backend::ReloadFrontend()
{
	emit(unloadFrontendRequest());
	// This is neccesary to force a reload of the file from disk
	mEngine.clearComponentCache();
	emit(loadFrontendRequest());
}

void Backend::handleTimer()
{
	while (true)
	{
		const size_t BUFFER_LEN = 2048;
		char buffer[BUFFER_LEN];

		if (!mUdpImuReceiver.DataAvailable())
			break;
		const size_t len = mUdpImuReceiver.Receive(buffer, BUFFER_LEN);

		const std::string bufferString(buffer, len);
		std::vector<std::string> strs;
		boost::split(strs, bufferString, boost::is_any_of(","));

		for (int i=0; i<strs.size(); ++i)
		{
			const std::string str = boost::algorithm::trim_copy(strs[i]);
			if (str == "81")
			{
				const std::string angleString = boost::algorithm::trim_copy(strs[i+3]);
				const double angle = boost::lexical_cast<double>(angleString);
				mVehicleDataModel.SetInputSteeringWheelAngle(angle);
				break;
			}
		}
	}

	while (true)
	{
		OpenDashboard::Common::InboundPacket packet;

		if (!mUdpReceiver.DataAvailable())
			break;
		const size_t len = mUdpReceiver.Receive(packet.GetData(), packet.GetBufferSize());

		Control control;
		VehicleDynamic vehicleDynamic;
		Gps gps;
		Powertrain powertrain;
		DriverInput driverInput;

		for (int i=0; i<packet.GetNumberOfMessages(); ++i)
		{
			MessageType messageType = static_cast<MessageType>(packet.GetNextMessageType());

			switch (messageType)
			{
			case CONTROL_MSG:
			{
				control = packet.GetNextMessage<Control>();
				mControlDataDynamicModel.SetState(static_cast<ControlDataDynamicModel::State>(control.state()));
				break;
			}
			case VEHICLE_DYNAMIC_MSG:
			{
				vehicleDynamic = packet.GetNextMessage<VehicleDynamic>();
				mVehicleDataModel.SetVehicleVelocity(vehicleDynamic.velocity());
				mVehicleDataModel.SetVehicleAx(vehicleDynamic.ax());
				mVehicleDataModel.SetVehicleAy(vehicleDynamic.ay());
				mVehicleDataModel.SetVehicleAz(vehicleDynamic.az());
				break;
			}
			case GPS_MSG:
			{
				gps = packet.GetNextMessage<Gps>();
				mVehicleDataModel.SetPositionLon(gps.lon());
				mVehicleDataModel.SetPositionLat(gps.lat());

				mControlDataDynamicModel.SetLat(gps.lat());
				mControlDataDynamicModel.SetLon(gps.lon());
				break;
			}
			case POWERTRAIN_MSG:
			{
				powertrain = packet.GetNextMessage<Powertrain>();

				mVehicleDataModel.SetEngineRpm(powertrain.rotation());
				mVehicleDataModel.SetEnginePowerOut(powertrain.engine_power_out());
				mVehicleDataModel.SetEngineTorque(powertrain.engine_torque());
				mVehicleDataModel.SetEngineFuelConsumptionAbs(powertrain.engine_fuel_consumption_abs());
				mVehicleDataModel.SetEngineFuelConsumptionAct(powertrain.engine_fuel_consumption_act());
				mVehicleDataModel.SetEngineFuelConsumptionAvg(powertrain.engine_fuel_consumption_avg());
				mVehicleDataModel.SetEngineFuelLevel(powertrain.engine_fuel_level());

				mVehicleDataModel.SetGearboxGear(powertrain.gear());
				mVehicleDataModel.SetGearboxPowerIn(powertrain.gearbox_power_in());
				mVehicleDataModel.SetGearboxPowerOut(powertrain.gearbox_power_out());
				mVehicleDataModel.SetGearboxRotationIn(powertrain.gearbox_rotation_in());
				mVehicleDataModel.SetGearboxRotationOut(powertrain.gearbox_rotation_out());
				break;
			}
			case DRIVER_INPUT_MSG:
			{
				driverInput = packet.GetNextMessage<DriverInput>();
				mVehicleDataModel.SetInputThrottle(driverInput.throttle());
				mVehicleDataModel.SetInputBrake(driverInput.brake());
				mVehicleDataModel.SetInputSteeringWheelAngle(driverInput.steering_wheel_angle());
				break;
			}
			default:
				break;
			}

			std::stringstream serializedMessages;
			serializedMessages << MessageToString(vehicleDynamic);
			serializedMessages << MessageToString(gps);
			serializedMessages << MessageToString(powertrain);
			serializedMessages << MessageToString(driverInput);

			mControlDataDynamicModel.SetSerializedData(QString::fromStdString(serializedMessages.str()));
		}

	}
}
