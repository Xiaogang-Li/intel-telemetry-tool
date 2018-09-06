cmake_minimum_required(VERSION 2.8)

project( telemetry )

include_directories ("${PROJECT_SOURCE_DIR}/../library/inc/")
link_directories("${PROJECT_SOURCE_DIR}/../build/lib")

set(SOURCES_
    telemetry.cpp
    display/bar.cpp
    display/displayable.cpp
    display/screen.cpp
    display/sequence.cpp)


include_directories(display)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")

add_executable( telemetry ${SOURCES_} ${HEADERS_} )

set_target_properties(telemetry PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries( telemetry libintem.so ncursesw)
