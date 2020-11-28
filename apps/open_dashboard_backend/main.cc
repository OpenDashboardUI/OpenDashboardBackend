// Copyright (C) 2020 twyleg
#include <open_dashboard_backend/backend.h>

#include <QObject>
#if defined(OPEN_DASHBOARD_WEBVIEW_AVAILABLE)
#include <QtWebView/QtWebView>
#endif

#include <iostream>


int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(common_ui_resources);
	Q_INIT_RESOURCE(backend_resources);
	Q_INIT_RESOURCE(svg_multilayer_extracted);
	Q_INIT_RESOURCE(map);

	std::cout << "Started open_dashboard_backend" << std::endl;

#if defined(OPEN_DASHBOARD_WEBVIEW_AVAILABLE)
	QtWebView::initialize();
#endif

	OpenDashboard::Backend::Backend backend(argc, argv);
	return backend.Run();
}
