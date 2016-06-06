find_package(SQLite3 REQUIRED)

include_directories(
    ${SQLite3_INCLUDE_DIRS}
)

set(COMMON_SRCS
    ${COMMON_DIR}/configparser.cpp
    ${COMMON_DIR}/defaultlogger.cpp
    ${COMMON_DIR}/launcher.cpp
    ${COMMON_DIR}/orchestrator.cpp
    ${COMMON_DIR}/utils.cpp
)

set(FLOCK_SRCS
    ${FLOCK_DIR}/disk.cpp
    ${FLOCK_DIR}/diskmanager.cpp
    ${FLOCK_DIR}/flock.cpp
    ${FLOCK_DIR}/flockmanager.cpp
    ${FLOCK_DIR}/gridmanager.cpp
)

set(GPS_SRCS
    ${GPS_DIR}/gpspoint.cpp
    ${GPS_DIR}/trajectory.cpp
)

set(DATACONNECTORS_SRCS
    ${DATACONNECTORS_DIR}/fileconnector.cpp
    ${DATACONNECTORS_DIR}/rawdatastring.cpp
    ${DATACONNECTORS_DIR}/rawdatasqlite3_stmt.cpp
    ${DATACONNECTORS_DIR}/sqlitedataconnector.cpp
)

set(DATADECODERS_SRCS
    ${DATADECODERS_DIR}/berlinmoddecoder.cpp
    ${DATADECODERS_DIR}/brinkhoffmntgdecoder.cpp
    ${DATADECODERS_DIR}/customdecoder.cpp
    ${DATADECODERS_DIR}/tdrivedecoder.cpp
    ${DATADECODERS_DIR}/truckspaperdecoder.cpp
)

set(DATALISTENERS_SRCS
    ${DATALISTENERS_DIR}/gpspointbuffererlistener.cpp
    ${DATALISTENERS_DIR}/gpspointlistener.cpp
)

set(DATAPROCESSORS_SRCS
    ${DATAPROCESSORS_DIR}/flockprocessor.cpp
    ${DATAPROCESSORS_DIR}/flockprocessorthread.cpp
    ${DATAPROCESSORS_DIR}/flockutils.cpp
    ${DATAPROCESSORS_DIR}/parallelflockprocessor.cpp
)

set(PATTERNS_SOURCES
    ${COMMON_SRCS}
    ${DATACONNECTORS_SRCS}
    ${DATADECODERS_SRCS}
    ${DATALISTENERS_SRCS}
    ${DATAPROCESSORS_SRCS}
    ${FLOCK_SRCS}
    ${GPS_SRCS}
    main.cpp
)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

add_subdirectory(sorter)

add_executable(patterns ${PATTERNS_SOURCES})
target_link_libraries(patterns ${SQLite3_LIBRARIES})

target_include_directories(patterns PRIVATE
    ${COMMON_DIR}
    ${DATACONNECTORS_DIR}
    ${DATADECODERS_DIR}
    ${DATALISTENERS_DIR}
    ${DATAMODEL_DIR}
    ${DATAPROCESSORS_DIR}
    ${FLOCK_DIR}
    ${GPS_DIR}
)

