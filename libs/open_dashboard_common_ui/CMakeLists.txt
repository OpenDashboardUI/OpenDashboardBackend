set(TARGET_NAME open_dashboard_common_ui_lib)


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
find_package(Qt5 COMPONENTS REQUIRED Core Qml Quick)


#
# generate buildtime data
#
extract_svg_layers(
	INPUT_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/resources/images/svg-multilayer/icons.svg
	OUTPUT_DIR
	${CMAKE_BINARY_DIR}/common-ui-svg-multilayer-extracted/
	QRC_FILENAME
	common_ui_svg_multilayer_extracted.qrc
)


#
# add source files to target
#
add_library(${TARGET_NAME}
	helper.cc
	helper.h
	map_generator.cc
	map_generator.h
	connection_manager.cc
	connection_manager.h
	resources/common_ui_resources.qrc
	${CMAKE_BINARY_DIR}/common-ui-svg-multilayer-extracted/common_ui_svg_multilayer_extracted.qrc
)


#
# link against libs
#
target_link_libraries(${TARGET_NAME}
	Qt::Core
	open_dashboard_common_lib
)
