cmake_minimum_required(VERSION 2.6)
project(libgraph)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

include_directories(
	${CMAKE_SOURCE_DIR}/include
)

add_executable(convert
	${CMAKE_SOURCE_DIR}/src/Convert.cpp
)
