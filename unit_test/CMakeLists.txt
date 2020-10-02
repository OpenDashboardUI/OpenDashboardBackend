set(TARGET_NAME test_open_dashboard_backend)


#
# set cmake settings
#
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


add_executable(${TARGET_NAME}
	main.cc
	packet_test.cc
	csv_reader_test.cc
	config_test.cc
)

target_link_libraries(${TARGET_NAME}
	open_dashboard_common_lib
	pthread
	gtest
)
