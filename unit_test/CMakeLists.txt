set(TARGET_NAME test_open_dashboard_backend)


#
# set cmake settings
#
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(GTest CONFIG REQUIRED)


add_executable(${TARGET_NAME}
	main.cc
	packet_test.cc
	csv_reader_test.cc
	config_test.cc
	helper_test.cc
	map_generator_test.cc
)

target_link_libraries(${TARGET_NAME}
	open_dashboard_common_lib
	open_dashboard_common_ui_lib
	GTest::gmock
	GTest::gtest
	GTest::gmock_main
	GTest::gtest_main
)
