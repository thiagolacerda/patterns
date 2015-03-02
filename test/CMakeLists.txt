find_package(Threads)

set(GOOGLETEST_BUILD_DIR ${CMAKE_BINARY_DIR}/test/googletest/)
set(GOOGLETEST_ROOT_DIR ${PROJECT_SOURCE_DIR}/googletest)
set(GOOGLETEST_SRC_DIR ${GOOGLETEST_ROOT_DIR}/googletest)

# googlemock build is on (which already includes gtest), so the the output goes to googlemock folder
set(GTEST_BUILD_DIR ${GOOGLETEST_BUILD_DIR}/googlemock/gtest)
# set(GOOGLEMOCK_SRC_DIR ${GOOGLETEST_ROOT_DIR}/googlemock) not needed for now
add_subdirectory(${GOOGLETEST_ROOT_DIR} ${GOOGLETEST_BUILD_DIR})

set(GTEST_MAIN_LIB ${GTEST_BUILD_DIR}/libgtest_main.a)
set(GTEST_LIB ${GTEST_BUILD_DIR}/libgtest.a)
set(TEST_BUILD_DEPENDENCIES ${GTEST_MAIN_LIB} ${GTEST_LIB} ${CMAKE_THREAD_LIBS_INIT})

include_directories(
    ${GOOGLETEST_SRC_DIR}/include
    # ${GOOGLEMOCK_SRC_DIR}/include not needed now
)

add_custom_target(googletest DEPENDS gtest_main gtest)

add_subdirectory(common)
