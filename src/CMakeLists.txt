find_package(SQLite3 REQUIRED)

set(COMMON_DIR common)
set(DATABASE_DIR database)
set(DECODER_DIR decoder)
set(FLOCK_DIR flock)
set(GPS_DIR gps)

include_directories(
    ${COMMON_DIR}
    ${DATABASE_DIR}
    ${DECODER_DIR}
    ${FLOCK_DIR}
    ${GPS_DIR}
    ${SQLite3_INCLUDE_DIRS}
)

set(COMMON_SRCS
    ${COMMON_DIR}/config.cpp
    ${COMMON_DIR}/factory.cpp
    ${COMMON_DIR}/manager.cpp
    ${COMMON_DIR}/utils.cpp
)

set(DATABASE_SRCS
    ${DATABASE_DIR}/filedbmanager.cpp
    ${DATABASE_DIR}/sqlitedbmanager.cpp
)

set(DECODER_SRCS
    ${DECODER_DIR}/lifemapdbdecoder.cpp
    ${DECODER_DIR}/trucksdbdecoder.cpp
    ${DECODER_DIR}/truckspaperdbdecoder.cpp
)

set(FLOCK_SRCS
    ${FLOCK_DIR}/disk.cpp
    ${FLOCK_DIR}/diskmanager.cpp
    ${FLOCK_DIR}/flock.cpp
    ${FLOCK_DIR}/flockmanager.cpp
    ${FLOCK_DIR}/grid.cpp
    ${FLOCK_DIR}/gridmanager.cpp
)

set(GPS_SRCS
    ${GPS_DIR}/gpspoint.cpp
    ${GPS_DIR}/gpspointtemporalprocessor.cpp
    ${GPS_DIR}/trajectory.cpp
)

set(PATTERNS_SOURCES
    ${COMMON_SRCS}
    ${DATABASE_SRCS}
    ${DECODER_SRCS}
    ${FLOCK_SRCS}
    ${GPS_SRCS}
    main.cpp
)

add_executable(patterns ${PATTERNS_SOURCES})
target_link_libraries(patterns ${SQLite3_LIBRARIES})
