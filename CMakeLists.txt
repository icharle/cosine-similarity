cmake_minimum_required(VERSION 3.0)
project(cosine CXX C)

set(CMAKE_CXX_STANDARD 11)
# set paddle
set(PADDLE_ROOT "paddle path")

message(STATUS "CXX compiler: ${CMAKE_CXX_COMPILER}, version: "
        "${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "C compiler: ${CMAKE_C_COMPILER}, version: "
        "${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION}")

macro(safe_set_static_flag)
    foreach (flag_var
            CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        if (${flag_var} MATCHES "/MD")
            string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
        endif (${flag_var} MATCHES "/MD")
    endforeach (flag_var)
endmacro()

if (NOT DEFINED PADDLE_ROOT)
    message(FATAL_ERROR "please set PADDLE_ROOT with -DPADDLE_ROOT=/path/paddle/lib")
endif ()

if (IS_ABSOLUTE ${PADDLE_ROOT})
    set(PADDLE_ABS_PATH ${PADDLE_ROOT})
else ()
    get_filename_component(PADDLE_ABS_PATH ${CMAKE_BINARY_DIR}/${PADDLE_ROOT}/ ABSOLUTE)
endif ()

message(STATUS "paddle include: ${PADDLE_ABS_PATH}/paddle/include")
message(STATUS "paddle lib: ${PADDLE_ABS_PATH}/paddle/lib")
include_directories(${PADDLE_ABS_PATH}/paddle/include)
set(PADDLE_LIB_THIRD_PARTY_PATH "${PADDLE_ABS_PATH}/third_party/install/")
include_directories("${PADDLE_LIB_THIRD_PARTY_PATH}protobuf/include")
include_directories("${PADDLE_LIB_THIRD_PARTY_PATH}glog/include")
include_directories("${PADDLE_LIB_THIRD_PARTY_PATH}gflags/include")
include_directories("${PADDLE_LIB_THIRD_PARTY_PATH}xxhash/include")

link_directories("${PADDLE_LIB_THIRD_PARTY_PATH}protobuf/lib")
link_directories("${PADDLE_LIB_THIRD_PARTY_PATH}glog/lib")
link_directories("${PADDLE_LIB_THIRD_PARTY_PATH}gflags/lib")
link_directories("${PADDLE_LIB_THIRD_PARTY_PATH}xxhash/lib")
link_directories("${PADDLE_ABS_PATH}/paddle/lib")

if (WIN32)
    add_definitions("/DGOOGLE_GLOG_DLL_DECL=")
    if (MSVC)
        option(MSVC_STATIC_CRT "use static C Runtime library by default" ON)
        set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /bigobj /MTd")
        set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /bigobj /MT")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /bigobj /MTd")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /bigobj /MT")
        if (WITH_STATIC_LIB)
            safe_set_static_flag()
            add_definitions(-DSTATIC_LIB)
        endif ()
    endif ()
else ()
    set(CMAKE_CXX_FLAGS "-O3 -g -pipe -W -Wall -Wno-unused-parameter -fPIC -fpermissive -std=gnu++11")
endif ()

# add mkldnn library if it exitsts
set(mkldnn_inc_path ${PADDLE_ABS_PATH}/third_party/install/mkldnn/include)
set(mkldnn_lib_path ${PADDLE_ABS_PATH}/third_party/install/mkldnn/lib)
if (EXISTS ${mkldnn_inc_path} AND EXISTS ${mkldnn_lib_path})
    message(STATUS "MKLDNN lib found.")
    include_directories(${PADDLE_ABS_PATH}/third_party/install/mkldnn/include)
    if (WIN32)
        set(MKLDNN_LIB ${mkldnn_lib_path}/mkldnn.lib)
    else (WIN32)
        set(MKLDNN_LIB ${mkldnn_lib_path}/libmkldnn.so.0)
    endif (WIN32)
else ()
    message(STATUS "MKLDNN lib not found.")
endif ()

# add mklml library if it exists
set(mklml_inc_path ${PADDLE_ABS_PATH}/third_party/install/mklml/include)
set(mklml_lib_path ${PADDLE_ABS_PATH}/third_party/install/mklml/lib)
if (EXISTS ${mklml_inc_path} AND EXISTS ${mklml_lib_path})
    message(STATUS "MKLML lib found.")
    include_directories(${PADDLE_ABS_PATH}/third_party/install/mklml/include)
    if (WIN32)
        set(MATH_LIB ${mklml_lib_path}/mklml${CMAKE_STATIC_LIBRARY_SUFFIX}
                ${mklml_lib_path}/libiomp5md${CMAKE_STATIC_LIBRARY_SUFFIX})
    else ()
        set(MATH_LIB ${mklml_lib_path}/libmklml_intel${CMAKE_SHARED_LIBRARY_SUFFIX}
                ${mklml_lib_path}/libiomp5${CMAKE_SHARED_LIBRARY_SUFFIX})
    endif ()
else ()
    message(STATUS "MKLML lib not found.")
endif ()

# add openblas library if it exists
set(openblas_inc_path ${PADDLE_ABS_PATH}/third_party/install/openblas/include)
set(openblas_lib_path ${PADDLE_ABS_PATH}/third_party/install/openblas/lib)
if (EXISTS ${openblas_inc_path} AND EXISTS ${openblas_lib_path})
    message(STATUS "openblas lib found.")
    include_directories(${PADDLE_ABS_PATH}/third_party/install/openblas/include)
    if (WIN32)
        set(MATH_LIB ${openblas_lib_path}/openblas${CMAKE_STATIC_LIBRARY_SUFFIX})
    else ()
        set(MATH_LIB ${openblas_lib_path}/libopenblas${CMAKE_STATIC_LIBRARY_SUFFIX})
    endif ()
else ()
    message(STATUS "openblas lib not found.")
endif ()

if (WITH_STATIC_LIB)
    set(DEPS ${PADDLE_ABS_PATH}/paddle/lib/libpaddle_fluid${CMAKE_STATIC_LIBRARY_SUFFIX})
else ()
    if (WIN32)
        set(DEPS ${PADDLE_ABS_PATH}/paddle/lib/paddle_fluid${CMAKE_SHARED_LIBRARY_SUFFIX})
    else ()
        set(DEPS ${PADDLE_ABS_PATH}/paddle/lib/libpaddle_fluid${CMAKE_SHARED_LIBRARY_SUFFIX})
    endif ()
endif ()

if (NOT WIN32)
    set(EXTERNAL_LIB "-ldl -lpthread")
    set(DEPS ${DEPS}
            ${MATH_LIB} ${MKLDNN_LIB}
            glog gflags protobuf xxhash
            ${EXTERNAL_LIB})
else ()
    set(DEPS ${DEPS}
            ${MATH_LIB} ${MKLDNN_LIB}
            glog gflags_static libprotobuf xxhash ${EXTERNAL_LIB})
    set(DEPS ${DEPS} libcmt shlwapi.lib)
endif (NOT WIN32)

# lac source
include_directories(lib)
file(GLOB LAC_SOURCE "lib/*.cpp")

# cosine source
include_directories(src)
file(GLOB COSINE_SOURCE "src/*.cpp")

message(STATUS "'${PROJECT_SOURCE_DIR}' output dir")
add_library(cosine ${LAC_SOURCE} ${COSINE_SOURCE})

# Compile
add_executable(main main.cpp)
set_target_properties(main PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
target_link_libraries(main cosine ${DEPS})

# output
install(TARGETS cosine DESTINATION ${PROJECT_SOURCE_DIR}/output/lib)
install(TARGETS main DESTINATION ${PROJECT_SOURCE_DIR}/output/bin)