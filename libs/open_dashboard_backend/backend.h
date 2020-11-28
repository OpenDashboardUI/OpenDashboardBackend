// Copyright (C) 2020 twyleg
#pragma once

#include "cli_arguments.h"

#include <open_dashboard_data_models/data_models.h>
#include <open_dashboard_common/udp_transceiver.h>

#include <QWidget>
#include <QObject>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTimer>

#include <filesystem>
#include <optional>

namespace OpenDashboard::Backend {

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
	OpenDashboard::DataModels::DataModel mDataModel;

	QApplication mApplication;
	QQmlApplicationEngine mEngine;
	QTimer mReceiveTimer;

	OpenDashboard::Common::UdpReceiver mUdpReceiver;
	OpenDashboard::Common::UdpReceiver mUdpImuReceiver;

private:

	void LoadStartscreen();
	void LoadFrontend(const QUrl& frontendMainFileUrl);
	void ReloadFrontend();

	void ReadNetworkImu();
	void ReadVehicleData();

public slots:

	void handleTimer();
	void handleOpenFileRequest(const QUrl& filepath);
	void handleReloadRequest();

signals:

	void unloadFrontendRequest();
	void loadFrontendRequest();

};

}
