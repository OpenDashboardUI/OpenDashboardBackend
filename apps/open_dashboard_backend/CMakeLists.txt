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
find_package(Qt5 COMPONENTS REQUIRED Core)
find_package(Qt5 COMPONENTS WebView QUIET)


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
# Add compile definitions
#
if(Qt5WebEngine_DIR AND Qt5WebView_DIR)
	message(STATUS "Webview and WebEngine available")
	add_compile_definitions(OPEN_DASHBOARD_WEBVIEW_AVAILABLE=1)
else()
	message(STATUS "Webview or WebEngine not available")
endif()


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
	open_dashboard_backend_lib
	Qt::WebView
)
