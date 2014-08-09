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

find_package(SQLite3 REQUIRED)

include_directories(
    ${SQLite3_INCLUDE_DIRS}
)

set(PATTERNS_SOURCES
    config.cpp
    factory.cpp
    gpspoint.cpp
    grid.cpp
    gridmanager.cpp
    lifemapdbdecoder.cpp
    main.cpp
    manager.cpp
    sqlitedbmanager.cpp
    trajectory.cpp
    trajectorymanager.cpp
    utils.cpp
)

add_executable(patterns ${PATTERNS_SOURCES})
target_link_libraries(patterns ${SQLite3_LIBRARIES})
