set(TARGET_NAME open_dashboard_backend)


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
find_package(Qt5 COMPONENTS Core Widgets Qml Quick Location REQUIRED)
find_package(Boost COMPONENTS REQUIRED program_options system)


#
# Setup WinDeployQt to create a deployable package for windows
#
win_deploy_qt(
	TARGET
		${TARGET_NAME}
	QML_DIR
		"${CMAKE_SOURCE_DIR}/libs/open_dashboard_backend/resources/qml"
	DEPLOY_DIRECTORY_NAME
		"OpenDashboardBackend_win_x64"
)


#
# add source files to target
#
add_executable(${TARGET_NAME}
	main.cc
	backend.h
	backend.cc
)


#
# link against libs
#
target_link_libraries(${TARGET_NAME}
	open_dashboard_idl
	open_dashboard_common_lib
	open_dashboard_backend_lib
	Qt::Core
	Qt::Widgets
	Qt::Qml
	Qt::Quick
	Qt::Location
	${Boost_LIBRARIES}
)
