// Copyright (C) 2020 twyleg
#pragma once

#include <open_dashboard_backend/data_models.h>

#include <open_dashboard_common/udp_transceiver.h>

#include <QWidget>
#include <QObject>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTimer>

#include <filesystem>

struct CliArguments
{
	CliArguments(int argc, char* argv[]);
	void ValidateRuntimeArguments();

	std::string mHostname;
	int mPort;
	std::filesystem::path mConfigFilePath;
	std::filesystem::path mMainQmlFilePath;

	bool mSidebarsDisabled;
};


class Backend : public QObject
{
	Q_OBJECT

public:

	Backend(int argc, char* argv[], QObject* parent = nullptr);
	int Run();

	int mArgc;
	char** mArgv;

	CliArguments cliArguments;
	QGuiApplication mApplication;
	OpenDashboard::Common::UdpReceiver mUdpReceiver;
	OpenDashboard::Common::UdpReceiver mUdpImuReceiver;

	ControlDataStaticModel mControlDataStaticModel;
	ControlDataDynamicModel mControlDataDynamicModel;
	VehicleDataModel mVehicleDataModel;

public slots:

	void HandleTimer();

};

