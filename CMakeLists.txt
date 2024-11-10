cmake_minimum_required(VERSION 3.10)
project(Odrive)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBUSB REQUIRED libusb-1.0)

include_directories(
	${LIBUSB_INCLUDE_DIRS}
	${CMAKE_CURRENT_SOURCE_DIR}
)

set(SOURCES
	main.cpp
	endpoint.cpp
)

add_executable(endpoint_init_test ${SOURCES})

target_link_libraries(endpoint_init_test
	${LIBUSB_LIBRARIES}
)

link_directories(${LIBUSB_LIBRARY_DIRS})
