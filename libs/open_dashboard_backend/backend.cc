// Copyright (C) 2020 twyleg
#include "backend.h"
#include "frontend_config.h"

#include <open_dashboard_common/config.h>
#include <open_dashboard_common/packet.h>

#include <open_dashboard.pb.h>

#include <google/protobuf/text_format.h>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include <iostream>
#include <string>

namespace OpenDashboard::Backend {

namespace {

QUrl QUrlFromPath(const std::filesystem::path& path)
{
	return QUrl::fromLocalFile(QString::fromStdString(std::filesystem::absolute(path).generic_string()));
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

	mDataModel.GetControlDataStatic()->SetSidebarsDisabled(cliArguments.mSidebarsDisabled);

#if defined(OPEN_DASHBOARD_WEBVIEW_AVAILABLE)
	mDataModel.GetControlDataStatic()->SetWebViewAvailable(true);
#else
	mDataModel.GetControlDataStatic()->SetWebViewAvailable(false);
#endif

	if (cliArguments.mConfigFilePath)
	{
		auto config = OpenDashboard::Common::Config::ReadConfig(*cliArguments.mConfigFilePath);
		mDataModel.GetControlDataStatic()->SetVideoChannelOnePath(QUrlFromPath(config.mVideoData[0]));
		mDataModel.GetControlDataStatic()->SetVideoChannelTwoPath(QUrlFromPath(config.mVideoData[1]));
		mDataModel.GetControlDataStatic()->SetVideoChannelThreePath(QUrlFromPath(config.mVideoData[2]));
	}

	mEngine.addImportPath("qrc:/");
	mEngine.rootContext()->setContextProperty("dataModel", &mDataModel);
	mEngine.rootContext()->setContextProperty("backend", this);
	mEngine.load(QUrl(QStringLiteral("qrc:/backend/qml/Application.qml")));

	if (cliArguments.mMainQmlFilePath)
		LoadFrontend(QUrlFromPath(*cliArguments.mMainQmlFilePath));
	else
		LoadStartscreen();

	QObject::connect(&mReceiveTimer, SIGNAL(timeout()), this, SLOT(handleTimer()));
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
	const QUrl qApplicationPathUrl("qrc:/backend/qml/LogoStartscreen.qml");

	QVector<QQmlContext::PropertyPair> newProperties = {
		{ QString("qApplication"), qApplicationPathUrl }
	};

	mEngine.rootContext()->setContextProperties(newProperties);
	emit(loadFrontendRequest());
}

void Backend::LoadFrontend(const QUrl& frontendMainFileUrl)
{
	const std::filesystem::path frontendConfigFilePath = frontendMainFileUrl.toLocalFile().toStdString();

	OpenDashboard::FrontendConfig frontendConfig(frontendConfigFilePath);

	const std::filesystem::path qApplicationPath = frontendConfig.mQmlMainFile;
	const std::filesystem::path qwd = std::filesystem::absolute(frontendConfigFilePath).parent_path();

	const QUrl qApplicationPathUrl = QUrlFromPath(qApplicationPath);
	const QUrl qwdUrl = QUrlFromPath(qwd);

	QVector<QQmlContext::PropertyPair> newProperties = {
		{ QString("qApplication"), qApplicationPathUrl },
		{ QString("qwd"), qwdUrl }
	};

	for (auto importPath: frontendConfig.mQmlImportPaths)
		mEngine.addImportPath(QUrlFromPath(importPath).toString());

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
	ReadNetworkImu();
	ReadVehicleData();
}

void Backend::ReadNetworkImu()
{
	// This function receives an orientation from an Android network IMU:
	// https://play.google.com/store/apps/details?id=de.lorenz_fenster.sensorstreamgps

	const std::string ORIENTATION_TAG = "81";

	while (mUdpImuReceiver.DataAvailable())
	{
		const size_t BUFFER_LEN = 2048;
		char buffer[BUFFER_LEN];

		const size_t len = mUdpImuReceiver.Receive(buffer, BUFFER_LEN);

		const std::string bufferString(buffer, len);
		std::vector<std::string> strs;
		boost::split(strs, bufferString, boost::is_any_of(","));

		for (int i=0; i<strs.size(); ++i)
		{
			const std::string str = boost::algorithm::trim_copy(strs[i]);
			if (str == ORIENTATION_TAG)
			{
				const std::string angleString = boost::algorithm::trim_copy(strs[i+3]);
				const double angle = boost::lexical_cast<double>(angleString);
				Proto::DriverInput driverInput;
				driverInput.set_steering_wheel_angle(angle);
				mDataModel.GetVehicleData()->GetDriverInput()->FromProto(driverInput);
				break;
			}
		}
	}
}

void Backend::ReadVehicleData()
{
	while (mUdpReceiver.DataAvailable())
	{
		OpenDashboard::Common::InboundPacket packet;

		const size_t len = mUdpReceiver.Receive(packet.GetData(), packet.GetBufferSize());

		for (int i=0; i<packet.GetNumberOfMessages(); ++i)
		{
			Proto::MessageType messageType = static_cast<Proto::MessageType>(packet.GetNextMessageType());

			switch (messageType)
			{
			case Proto::CONTROL_MSG:
			{
				auto msg = packet.GetNextMessage<Proto::Control>();
				mDataModel.GetControlDataDynamic()->SetState(static_cast<DataModels::ControlDataDynamic::State>(msg.state()));
				break;
			}
			case Proto::DYNAMICS_MSG:
			{
				auto msg = packet.GetNextMessage<Proto::Dynamics>();
				mDataModel.GetVehicleData()->GetDynamics()->FromProto(msg);
				break;
			}
			case Proto::GPS_MSG:
			{
				auto msg = packet.GetNextMessage<Proto::Gps>();
				mDataModel.GetVehicleData()->GetGps()->FromProto(msg);
				break;
			}
			case Proto::POWERTRAIN_MSG:
			{
				auto msg = packet.GetNextMessage<Proto::Powertrain>();
				mDataModel.GetVehicleData()->GetPowertrain()->FromProto(msg);
				break;
			}
			case Proto::DRIVER_INPUT_MSG:
			{
				auto msg = packet.GetNextMessage<Proto::DriverInput>();
				mDataModel.GetVehicleData()->GetDriverInput()->FromProto(msg);
				break;
			}
			default:
				break;
			}

			mDataModel.SerializeQuantities();
		}

	}
}

}
