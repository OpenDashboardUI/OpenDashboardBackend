set(TARGET_NAME open_dashboard_harddisk_player)


#
# set cmake settings
#
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


#
# find packages
#
find_package(Boost COMPONENTS REQUIRED program_options system)


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
	open_dashboard_common_lib
	open_dashboard_idl
	${Boost_LIBRARIES}
)
