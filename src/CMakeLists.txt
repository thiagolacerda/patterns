find_package(SQLite3 REQUIRED)

include_directories(
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
    ${DECODER_DIR}/basegpsfiledecoder.cpp
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

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

add_executable(patterns ${PATTERNS_SOURCES})
target_link_libraries(patterns ${SQLite3_LIBRARIES})
