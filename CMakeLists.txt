cmake_minimum_required(VERSION 3.1.0)

project(OpenDashboardBackend)

include(cmake/win_deploy_qt.cmake)
include(cmake/google_maps_key_template_engine.cmake)
include(external/svg_layer_extractor/cmake/svg_layer_extractor.cmake)

#
# Add subfolders
#
add_subdirectory(external/OpenDashboardIdl/)

add_subdirectory(libs/open_dashboard_common/)
add_subdirectory(libs/open_dashboard_common_ui/)
add_subdirectory(libs/open_dashboard_data_models/)
add_subdirectory(libs/open_dashboard_backend/)
add_subdirectory(libs/open_dashboard_control_panel/)

add_subdirectory(apps/open_dashboard_backend/)
add_subdirectory(apps/open_dashboard_harddisk_player/)
add_subdirectory(apps/open_dashboard_control_panel/)

add_subdirectory(unit_test/)
