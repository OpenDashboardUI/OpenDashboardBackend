// Copyright (C) 2020 twyleg
#pragma once

#include <open_dashboard_backend/data_models.h>

#include <open_dashboard_common/udp_transceiver.h>

#include <QWidget>
#include <QObject>
#include <QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTimer>

#include <filesystem>
#include <optional>

struct CliArguments
{
	CliArguments(int argc, char* argv[]);
	void ValidateRuntimeArguments();

	std::string mHostname;
	int mPort;
	std::optional<std::filesystem::path> mConfigFilePath;
	std::optional<std::filesystem::path> mMainQmlFilePath;

	bool mSidebarsDisabled;
};


class Backend : public QObject
{
	Q_OBJECT

public:

	Backend(int argc, char* argv[], QObject* parent = nullptr);
	~Backend();
	int Run();

	int mArgc;
	char** mArgv;

	CliArguments cliArguments;

	ControlDataStaticModel mControlDataStaticModel;
	ControlDataDynamicModel mControlDataDynamicModel;
	VehicleDataModel mVehicleDataModel;

	QApplication mApplication;
	QQmlApplicationEngine mEngine;
	QTimer mReceiveTimer;

	OpenDashboard::Common::UdpReceiver mUdpReceiver;
	OpenDashboard::Common::UdpReceiver mUdpImuReceiver;

private:

	void LoadStartscreen();
	void LoadFrontend(const QUrl& frontendMainFileUrl);
	void ReloadFrontend();

public slots:

	void handleTimer();
	void handleOpenFileRequest(const QUrl& filepath);
	void handleReloadRequest();

signals:

	void unloadFrontendRequest();
	void loadFrontendRequest();

};

