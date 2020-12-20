// Copyright (C) 2020 twyleg
#include <open_dashboard_control_panel/control_panel.h>

#include <QObject>

#include <iostream>

int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(common_ui_resources);
	Q_INIT_RESOURCE(common_ui_svg_multilayer_extracted);
	Q_INIT_RESOURCE(control_panel_resources);

	std::cout << "Started" << std::endl;

	OpenDashboard::ControlPanel::ControlPanel controlPanel(argc, argv);
	return controlPanel.Run();
}
