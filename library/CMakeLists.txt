cmake_minimum_required(VERSION 2.8)

project( intem )

set(SOURCES_
    cpu.cpp
    cpuinfo.cpp
    igfx.c
    gtutilization.cpp
    gtinfo.cpp
    gtinfo_by_cl.cpp
    clinfo.cpp
    gtmemory.cpp
    mediacapability.cpp
    sysmemory.cpp
 )

set(HEADERS_
    inc/cpu.h
    inc/cpuinfo.h
    inc/i915_pciids.h
    inc/igfx.h
    inc/gtinfo.h
    inc/gtinfo_by_cl.h
    inc/clinfo.h
    inc/gtmemory.h
    inc/mediacapability.h
    inc/sysmemory.h 
    inc/util.h
)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")

add_library(intem SHARED ${SOURCES_} ${HEADERS_})

set_target_properties(intem PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(intem va va-drm OpenCL pciaccess)

include_directories("./inc")
