set(TEST_UTILS_DEPENDENCIES
    ${COMMON_DIR}/utils.cpp
    ${COMMON_DIR}/config.cpp
    ${GPS_DIR}/gpspoint.cpp
)

create_test_target(test_utils test_utils.cpp "${TEST_UTILS_DEPENDENCIES}" "")
