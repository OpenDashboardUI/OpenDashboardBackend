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
# add source files to target
#
add_library(${TARGET_NAME}
	resources/common_ui_resources.qrc
)


#
# link against libs
#
target_link_libraries(${TARGET_NAME}
	Qt::Core
)
