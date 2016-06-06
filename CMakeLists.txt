cmake_minimum_required(VERSION 2.8)
project(patterns)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wunused-parameter")

if(UNIX AND NOT APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
elseif(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wthread-safety -stdlib=libc++")
    add_compile_options("-pthread")
endif()

string(TOLOWER "${CMAKE_BUILD_TYPE}" BuildTypeLower)
if(${BuildTypeLower} STREQUAL debug)
    string(REPLACE "-g" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3")
    set(BuildType Debug)
else(${BuildTypeLower} STREQUAL debug)
    set(CMAKE_BUILD_TYPE Release)
    set(BuildType Release)
endif(${BuildTypeLower} STREQUAL debug)

message(STATUS "Build type: " ${BuildType})

set(PROJECT_SRC_DIR ${PROJECT_SOURCE_DIR}/src)

set(COMMON_DIR ${PROJECT_SRC_DIR}/common)
set(DATABASE_DIR ${PROJECT_SRC_DIR}/database)
set(DECODER_DIR ${PROJECT_SRC_DIR}/decoder)
set(FLOCK_DIR ${PROJECT_SRC_DIR}/flock)
set(GPS_DIR ${PROJECT_SRC_DIR}/gps)
set(PERFORMANCE_DIR ${PROJECT_SRC_DIR}/performance)

add_subdirectory(src)
enable_testing()
add_subdirectory(test)
