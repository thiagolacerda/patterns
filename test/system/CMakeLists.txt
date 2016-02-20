find_package(SQLite3 REQUIRED)

include_directories(
    ${SQLite3_INCLUDE_DIRS}
)

set(TEST_SYSTEM_DEPENDENCIES
    ${COMMON_DIR}/config.cpp
    ${COMMON_DIR}/factory.cpp
    ${COMMON_DIR}/manager.cpp
    ${COMMON_DIR}/utils.cpp
    ${DATABASE_DIR}/filedbmanager.cpp
    ${DATABASE_DIR}/sqlitedbmanager.cpp
    ${DECODER_DIR}/basegpsfiledecoder.cpp
    ${DECODER_DIR}/berlinmoddecoder.cpp
    ${DECODER_DIR}/lifemapdbdecoder.cpp
    ${DECODER_DIR}/sortedfiledecoder.cpp
    ${DECODER_DIR}/tdrivedecoder.cpp
    ${DECODER_DIR}/trucksdbdecoder.cpp
    ${DECODER_DIR}/truckspaperdbdecoder.cpp
    ${FLOCK_DIR}/disk.cpp
    ${FLOCK_DIR}/diskmanager.cpp
    ${FLOCK_DIR}/flock.cpp
    ${FLOCK_DIR}/flockmanager.cpp
    ${FLOCK_DIR}/grid.cpp
    ${FLOCK_DIR}/gridmanager.cpp
    ${GPS_DIR}/gpspoint.cpp
    ${GPS_DIR}/gpspointtemporalprocessor.cpp
    ${GPS_DIR}/trajectory.cpp
)

if(TEST_THOROUGH)
    add_definitions(-DTEST_THOROUGH)
endif()

create_test_target(test_system test_system.cpp "${TEST_SYSTEM_DEPENDENCIES}" "${SQLite3_LIBRARIES}")
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
