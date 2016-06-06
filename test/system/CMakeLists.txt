find_package(SQLite3 REQUIRED)

include_directories(
    ${COMMON_DIR}
    ${DATACONNECTORS_DIR}
    ${DATADECODERS_DIR}
    ${DATALISTENERS_DIR}
    ${DATAMODEL_DIR}
    ${DATAPROCESSORS_DIR}
    ${FLOCK_DIR}
    ${GPS_DIR}
    ${SQLite3_INCLUDE_DIRS}
)

set(TEST_SYSTEM_DEPENDENCIES
    ${COMMON_DIR}/configparser.cpp
    ${COMMON_DIR}/orchestrator.cpp
    ${COMMON_DIR}/utils.cpp
    ${DATACONNECTORS_DIR}/fileconnector.cpp
    ${DATACONNECTORS_DIR}/rawdatastring.cpp
    ${DATADECODERS_DIR}/truckspaperdecoder.cpp
    ${DATALISTENERS_DIR}/gpspointbuffererlistener.cpp
    ${DATALISTENERS_DIR}/gpspointlistener.cpp
    ${DATAPROCESSORS_DIR}/flockutils.cpp
    ${DATAPROCESSORS_DIR}/flockprocessor.cpp
    ${FLOCK_DIR}/disk.cpp
    ${FLOCK_DIR}/diskmanager.cpp
    ${FLOCK_DIR}/flock.cpp
    ${FLOCK_DIR}/flockmanager.cpp
    ${FLOCK_DIR}/gridmanager.cpp
    ${GPS_DIR}/gpspoint.cpp
    ${GPS_DIR}/trajectory.cpp
)

if(TEST_THOROUGH)
    add_definitions(-DTEST_THOROUGH)
endif()

create_test_target(test_system test_system.cpp "${TEST_SYSTEM_DEPENDENCIES}" "${SQLite3_LIBRARIES}")
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
