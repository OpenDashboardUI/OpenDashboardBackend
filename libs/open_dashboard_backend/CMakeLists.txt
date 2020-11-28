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
find_package(Qt5 COMPONENTS REQUIRED Core Widgets Qml Quick Location Charts)
find_package(Qt5 COMPONENTS WebView QUIET)
find_package(Boost COMPONENTS REQUIRED program_options system)


#
# generate buildtime data
#
extract_svg_layers(
	INPUT_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/resources/svg-multilayer/sidebar_button.svg
	${CMAKE_CURRENT_SOURCE_DIR}/resources/svg-multilayer/buttons.svg
	OUTPUT_DIR
	${CMAKE_BINARY_DIR}/svg-multilayer-extracted/
)

generate_google_maps_html_file(
	INPUT_FILE
	${CMAKE_CURRENT_SOURCE_DIR}/resources/html/map.html_template
	OUTPUT_FILE
	${CMAKE_BINARY_DIR}/map.html
	API_KEY
	${GOOGLE_MAPS_API_KEY}
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
add_library(${TARGET_NAME}
	backend.cc
	backend.h
	frontend_config.cc
	frontend_config.h
	cli_arguments.cc
	cli_arguments.h

	resources/backend_resources.qrc
	${CMAKE_BINARY_DIR}/svg-multilayer-extracted/svg_multilayer_extracted.qrc
	${CMAKE_BINARY_DIR}/map.qrc
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
	${Boost_LIBRARIES}
#	stdc++fs
)


#
# add include folder
#
target_include_directories(${TARGET_NAME}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..
)
