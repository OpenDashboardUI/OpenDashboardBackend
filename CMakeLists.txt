cmake_minimum_required(VERSION 3.1.0)

project(open_dashboard_backend)

include(cmake/win_deploy_qt.cmake)
include(external/svg_layer_extractor/cmake/svg_layer_extractor.cmake)

#
# Add subfolders
#
add_subdirectory(external/OpenDashboardIdl/)

add_subdirectory(libs/open_dashboard_backend/)
add_subdirectory(libs/open_dashboard_common/)

add_subdirectory(apps/open_dashboard_backend/)
add_subdirectory(apps/open_dashboard_harddisk_player/)

add_subdirectory(unit_test/)
