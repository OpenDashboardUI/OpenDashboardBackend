set(TARGET_NAME open_dashboard_backend_lib)


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
find_package(Qt5 COMPONENTS REQUIRED Core Widgets Qml Quick Location Charts QuickControls2)
find_package(Qt5 COMPONENTS WebView WebEngine QUIET)
find_package(Boost COMPONENTS REQUIRED program_options system)


#
# generate buildtime data
#
extract_svg_layers(
	INPUT_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/resources/svg-multilayer/controlbar_button.svg
	OUTPUT_DIR
	${CMAKE_BINARY_DIR}/backend-svg-multilayer-extracted/
	QRC_FILENAME
	backend_svg_multilayer_extracted.qrc
)


#
# Add compile definitions
#
if(Qt5WebEngine_DIR AND Qt5WebView_DIR)
	message(STATUS "Webview and WebEngine available")
	add_compile_definitions(OPEN_DASHBOARD_WEBVIEW_AVAILABLE=1)
else()
	message(STATUS "Webview (${Qt5WebView_DIR}) or WebEngine (${Qt5WebEngine_DIR}) not available")

endif()

if(USE_RELATIVE_QML_FILES)
	message(STATUS "Using relative middleware QML files")
	add_compile_definitions(USE_RELATIVE_QML_FILES=1)
	add_compile_definitions(OPEN_DASHBOARD_SOURCE_DIR="${CMAKE_SOURCE_DIR}")
else()
	message(STATUS "Using QRC middleware QML files")
endif()

#
# add source files to target
#
add_library(${TARGET_NAME}
	backend.cc
	backend.h
	frontend_config.cc
	frontend_config.h
	cli_arguments.cc
	cli_arguments.h

	resources/backend_resources.qrc
	${CMAKE_BINARY_DIR}/backend-svg-multilayer-extracted/backend_svg_multilayer_extracted.qrc
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
	Qt::Location
	Qt::Charts
	Qt::WebView
	Qt::QuickControls2
	${Boost_LIBRARIES}
#	stdc++fs
)


#
# add include folder
#
target_include_directories(${TARGET_NAME}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..
)
