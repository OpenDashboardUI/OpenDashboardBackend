set(TARGET_NAME open_dashboard_control_panel)


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
find_package(Qt5 COMPONENTS Core REQUIRED)


#
# add source files to target
#
add_executable(${TARGET_NAME}
	main.cc
)


#
# link against libs
#
target_link_libraries(${TARGET_NAME}
	open_dashboard_control_panel_lib
)
