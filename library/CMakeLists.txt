cmake_minimum_required(VERSION 2.8)

project( intem )

set(SOURCES_
    cpu.cpp
    cpuinfo.cpp
    gtutilization.cpp
    gtinfo.cpp
    gtinfo_by_cl.cpp
    gtmemory.cpp
    mediacapability.cpp
    sysmemory.cpp
    intem.cpp
    intem_imp.cpp
 )

set(HEADERS_
    inc/cpu.h
    inc/cpuinfo.h
    inc/i915_pciids.h
    inc/gtinfo.h
    inc/gtinfo_by_cl.h
    inc/gtmemory.h
    inc/mediacapability.h
    inc/sysmemory.h 
    inc/util.h
    inc/intem.h
    inc/intem_imp.h
)

find_library(VALIBRARY va)
find_library(DRMLIBRARY drm)
find_library(OPENCLLIBRARY OpenCL)
find_library(PCIACCESS pciaccess)

if((${VALIBRARY} STREQUAL "VALIBRARY-NOTFOUND") OR (${DRMLIBRARY} STREQUAL "DRMLIBRARY-NOTFOUND"))
    Message("va or va-drm library is not installed.")
    Message("The intel driver capability will can\nnot be shown. If you need the feature,\nplease run the script preinstall.sh\nwhich is located at the 'build' directory.\n")
endif()

set(OPENCL_ENALBED FALSE)
if(${OPENCLLIBRARY} STREQUAL "OPENCLLIBRARY-NOTFOUND")
    Message("OpenCL library is not installed.")
    Message("Some CPU/GPU static information will can \nnot be shown. If you need the infomation,\nplease run the script preinstall.sh\nwhich is located at the 'build' directory.\n")
else()
    add_definitions(-DOPENCL_ENABLED)
    set(OPENCL_ENALBED TRUE)
endif()

if(${PCIACCESS} STREQUAL "PCIACCESS-NOTFOUND")
    Message("pciaccess library is not installed.")
    Message("Some GPU dynamic information will can\nnot be shown. If you need the infomation,\nplease run the script preinstall.sh\nwhich is located at the 'build' directory.\n")
endif()

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -o2 -std=c++11 -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security -fstack-protector-strong -fPIE -fPIC")
set (CMAKE_EXE_LINKER_FLAGS "-z noexecstack -z relro -z now ")


if(${OPENCL_ENABLED})
set(SOURCES_
    ${SOURCES_}
    clinfo.cpp
)

set(HEADERS_
    ${HEADERS_}
    inc/clinfo.h
)
endif()

add_library(intem SHARED ${SOURCES_} ${HEADERS_})

set_target_properties(intem PROPERTIES LINKER_LANGUAGE CXX)

set(REQUIRE_LIBS
    va 
    va-drm
    pciaccess
    pthread
)

#target_link_libraries(intem va va-drm OpenCL pciaccess pthread)

if(${OPENCL_ENABLED})
    set(REQUIRE_LIBS
        ${REQUIRE_LIBS}
        OpenCL)
endif()

Message(${REQUIRE_LIBS})
target_link_libraries(intem ${REQUIRE_LIBS})

include_directories("./inc")
