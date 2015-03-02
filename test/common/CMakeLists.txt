
set(COMMON_TESTS
    test_utils.cpp
)

add_executable(test_utils ${COMMON_TESTS} ${COMMON_DIR}/utils.cpp ${COMMON_DIR}/config.cpp)
target_link_libraries(test_utils ${TEST_BUILD_DEPENDENCIES})
add_test(test_utils test_utils)
add_dependencies(test_utils googletest)
