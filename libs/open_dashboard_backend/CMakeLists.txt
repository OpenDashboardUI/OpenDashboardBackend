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
find_package(Qt5 COMPONENTS REQUIRED Core Qml Quick Location)


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
# add source files to target
#
add_library(${TARGET_NAME}
	data_models.h
	resources/application.qrc
	${CMAKE_BINARY_DIR}/svg-multilayer-extracted/svg_multilayer_extracted.qrc
	${CMAKE_BINARY_DIR}/map.qrc
)


#
# link against libs
#
target_link_libraries(${TARGET_NAME}
	Qt::Core
)


#
# add include folder
#
target_include_directories(${TARGET_NAME}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..
)
