// Copyright (C) 2020 twyleg
#pragma once

#include "harddisk_player_controller.h"

#include <open_dashboard_data_models/data_models.h>
#include <open_dashboard_common_ui/connection_manager.h>

#include <QWidget>
#include <QObject>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTimer>

namespace OpenDashboard::ControlPanel {

class ControlPanel : public QObject
{
	Q_OBJECT

	int mArgc;
	char** mArgv;

	template<class T>
	void Send(const T& t);

public:

	ControlPanel(int argc, char* argv[], QObject* parent = 0);

	int Run();

	QApplication mApplication;
	DataModels::DataModel mDataModel;
	CommonUI::ConnectionManager mConnectionManager;
	HarddiskPlayerController mHarddiskPlayerController;

};

}
