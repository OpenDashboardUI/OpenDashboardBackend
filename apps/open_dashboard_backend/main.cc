// Copyright (C) 2020 twyleg
#include "backend.h"

#include <open_dashboard_common/udp_transceiver.h>

#include <open_dashboard.pb.h>

#include <QWidget>
#include <QObject>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTimer>

#if defined(OPEN_DASHBOARD_WEBVIEW_AVAILABLE)
#include <QtWebView/QtWebView>
#endif

#include <iostream>


int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(application);
	Q_INIT_RESOURCE(svg_multilayer_extracted);
	Q_INIT_RESOURCE(map);

	std::cout << "Started open_dashboard_backend" << std::endl;

#if defined(OPEN_DASHBOARD_WEBVIEW_AVAILABLE)
	QtWebView::initialize();
#endif

	Backend backend(argc, argv);
	return backend.Run();
}
