// Copyright (C) 2020 twyleg
#include "backend.h"

#include <open_dashboard_common/config.h>
#include <open_dashboard_common/packet.h>

#include <open_dashboard.pb.h>

#include <google/protobuf/text_format.h>

#include <boost/program_options.hpp>

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
		("config,c", po::value<std::filesystem::path>(&mConfigFilePath)->required(), "Config")
		("qml-file", po::value<std::filesystem::path>(&mMainQmlFilePath)->required(), "QML main file")
	;

	po::positional_options_description p;
	p.add("qml-file", -1);

	try {
		po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
		po::notify(vm);
	} catch (const boost::wrapexcept<boost::program_options::required_option>& e) {
		std::cout << "Missing required argument " << e.get_option_name() << std::endl;
		std::cout << desc << std::endl;
		std::exit(-1);
	}

	if (vm.count("help")) {
		std::cout << desc << std::endl;
		std::exit(0);
	}
}

void CliArguments::ValidateRuntimeArguments()
{
	THROW_IF(!std::filesystem::is_regular_file(mConfigFilePath), "Config file does not exist: {}", mConfigFilePath.string());
	THROW_IF(mConfigFilePath.extension() != ".xml", "Config file does not have \".xml\" extension: {}", mConfigFilePath.string());

	THROW_IF(!std::filesystem::is_regular_file(mMainQmlFilePath), "Qml file does not exist: {}", mMainQmlFilePath.string());
	THROW_IF(mMainQmlFilePath.extension() != ".qml", "Qml file does not have \".qml\" extension: {}", mMainQmlFilePath.string());
}

Backend::Backend(int argc, char* argv[], QObject* parent)
:	QObject(parent),
	mArgc(argc),
	mArgv(argv),
	cliArguments(mArgc, mArgv),
	mApplication(mArgc, mArgv),
	mUdpReceiver(cliArguments.mHostname, cliArguments.mPort)
{;
	cliArguments.ValidateRuntimeArguments();

	mUdpReceiver.SetNonBlocking();
	mUdpReceiver.EnableReuseAddr();
}

int Backend::Run()
{
	auto config = OpenDashboard::Common::Config::ReadConfig(cliArguments.mConfigFilePath);

	const std::filesystem::path qApplicationPath = cliArguments.mMainQmlFilePath;
	const std::filesystem::path qwd = qApplicationPath.parent_path();

	QQmlApplicationEngine engine;
	engine.addImportPath("qrc:/");

	mControlDataStaticModel.SetVideoChannelOnePath(QString::fromStdString(
			std::filesystem::absolute(config.mVideoData[0])));
	mControlDataStaticModel.SetVideoChannelTwoPath(QString::fromStdString(
			std::filesystem::absolute(config.mVideoData[1])));
	mControlDataStaticModel.SetVideoChannelThreePath(QString::fromStdString(
			std::filesystem::absolute(config.mVideoData[2])));

	engine.rootContext()->setContextProperty("controlDataStaticModel", &mControlDataStaticModel);
	engine.rootContext()->setContextProperty("controlDataDynamicModel", &mControlDataDynamicModel);
	engine.rootContext()->setContextProperty("dataModel", &mVehicleDataModel);

	engine.rootContext()->setContextProperty("qwd", QString::fromStdString(
			"file:///" + std::filesystem::absolute(qwd).string()));
	engine.rootContext()->setContextProperty("qApplication", QString::fromStdString(
			"file:///" + std::filesystem::absolute(qApplicationPath).string()));
	engine.load(QUrl(QStringLiteral("qrc:/qml/Application.qml")));

	QTimer *timer = new QTimer(this);
	QObject::connect(timer,SIGNAL(timeout()), this, SLOT(HandleTimer()));
	timer->start(10);

	return mApplication.exec();
}

void Backend::HandleTimer()
{

	while (true)
	{
		OpenDashboard::Common::InboundPacket packet;
		ssize_t len = 0;
		if ((len = mUdpReceiver.Receive(packet.GetData(), packet.GetBufferSize())) <= 0)
			break;

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
