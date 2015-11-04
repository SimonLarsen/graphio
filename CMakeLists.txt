cmake_minimum_required(VERSION 2.6)
project(libgraph)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

if(NOT CMAKE_BUILD_TYPE)
	message(STATUS "No build type selected. Defaulting to Release.")
	set(CMAKE_BUILD_TYPE "Release")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

find_package(Boost REQUIRED)

include_directories(
	${CMAKE_SOURCE_DIR}/include
	${Boost_INCLUDE_DIRS}
)

add_executable(convert
	${CMAKE_SOURCE_DIR}/src/Convert.cpp
)
