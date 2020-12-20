set(TARGET_NAME open_dashboard_control_panel_lib)


#
# set cmake settings
#
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


#
# find packages
#
find_package(Qt5 COMPONENTS Core Widgets Qml Quick QuickControls2 REQUIRED)


add_library(${TARGET_NAME}
	control_panel.cc
	control_panel.h
	harddisk_player_controller.cc
	harddisk_player_controller.h
	resources/control_panel_resources.qrc
)


#
# link against libs
#
target_link_libraries(${TARGET_NAME}
	open_dashboard_idl
	open_dashboard_common_lib
	open_dashboard_common_ui_lib
	open_dashboard_data_models_lib
	Qt::Core
	Qt::Widgets
	Qt::Qml
	Qt::Quick
	Qt::QuickControls2
)


target_include_directories(${TARGET_NAME}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..
)
