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

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O2 -std=c++11 -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security -fstack-protector-strong -fPIE -fPIC")
set (CMAKE_EXE_LINKER_FLAGS "-z noexecstack -z relro -z now ")

add_executable( telemetry ${SOURCES_} ${HEADERS_} )

set_target_properties(telemetry PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries( telemetry libintem.so ncurses pthread)
