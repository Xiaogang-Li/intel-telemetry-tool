cmake_minimum_required(VERSION 2.8)

project( telemetry )

include_directories ("${PROJECT_SOURCE_DIR}/../library")
link_directories("${PROJECT_SOURCE_DIR}/../build/lib")

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")

add_executable( telemetry telemetry.cpp )

set_target_properties(telemetry PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries( telemetry intem)
