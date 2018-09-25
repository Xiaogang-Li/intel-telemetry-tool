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

find_library(VALIBRARY, "va")
find_library(DRMLIBRARY, "va-drm")
find_library(OPENCLLIBRARY, "OpenCL")
find_library(PCIACCESS, "pciaccess")

if((NOT $(VALIBRARY)) OR (NOT $(DRMLIBRARY)))
    Message("va or va-drm library is not installed.")
    Message("The intel driver capability will can\nnot be shown. If you need the feature,\nplease run the script preinstall.sh\nwhich is located at the 'build' directory.\n")
endif()

if((NOT $(OPENCLLIBRARY)))
    Message("OpenCL library is not installed.")
    Message("Some CPU/GPU static information will can \nnot be shown. If you need the infomation,\nplease run the script preinstall.sh\nwhich is located at the 'build' directory.\n")
endif()

if((NOT $(PCIACCESS)))
    Message("pciaccess library is not installed.")
    Message("Some GPU dynamic information will can\nnot be shown. If you need the infomation,\nplease run the script preinstall.sh\nwhich is located at the 'build' directory.\n")
endif()

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")

add_library(intem SHARED ${SOURCES_} ${HEADERS_})

set_target_properties(intem PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(intem va va-drm OpenCL pciaccess)

include_directories("./inc")
