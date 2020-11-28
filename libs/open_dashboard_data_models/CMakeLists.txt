set(TARGET_NAME open_dashboard_data_models_lib)


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


#
# add source files to target
#
add_library(${TARGET_NAME}
	data_models.h
	data_models.cc
	helper.h
)


#
# link against libs
#
target_link_libraries(${TARGET_NAME}
	Qt::Core
	open_dashboard_idl
)


#
# add include folder
#
target_include_directories(${TARGET_NAME}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..
)
