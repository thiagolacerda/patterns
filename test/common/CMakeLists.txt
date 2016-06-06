include_directories(
    ${COMMON_DIR}
    ${GPS_DIR}
)

set(TEST_COMMON_DEPENDENCIES
    ${COMMON_DIR}/utils.cpp
    ${GPS_DIR}/gpspoint.cpp
)

set(TEST_CONFIGPARSER_DEPENDENCIES
    ${COMMON_DIR}/configparser.cpp
    ${TEST_COMMON_DEPENDENCIES}
)

create_test_target(test_utils utilstest.cpp "${TEST_COMMON_DEPENDENCIES}" "")
create_test_target(test_configparser configparsertest.cpp "${TEST_CONFIGPARSER_DEPENDENCIES}" "")
