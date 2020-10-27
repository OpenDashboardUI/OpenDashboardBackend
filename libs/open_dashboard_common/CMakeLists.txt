set(TARGET_NAME open_dashboard_common_lib)

#
# set cmake settings
#
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


#
# find packages
#
find_package(Qt5 COMPONENTS Core Widgets Qml Quick Location REQUIRED)
find_package(XercesC REQUIRED)
find_package(fmt REQUIRED)
find_path(BOOST_ASIO_INCLUDE_DIRS "boost/asio.hpp")


#
# add source files to target
#
add_library(${TARGET_NAME}
	udp_transceiver.cc
	udp_transceiver.h
	packet.cc
	packet.h
	config.cc
	config.h
	csv_reader.cc
	csv_reader.h
)


#
# link against libs
#
target_link_libraries(${TARGET_NAME}
	open_dashboard_idl
	XercesC::XercesC
	fmt::fmt
)

target_include_directories(${TARGET_NAME}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..
	${BOOST_ASIO_INCLUDE_DIRS}
)
