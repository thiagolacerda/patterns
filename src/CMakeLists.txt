find_package(SQLite3 REQUIRED)

include_directories(
    ${SQLite3_INCLUDE_DIRS}
)

add_subdirectory(sorter)

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
    ${DECODER_DIR}/berlinmoddecoder.cpp
    ${DECODER_DIR}/brinkhoffmntgdecoder.cpp
    ${DECODER_DIR}/lifemapdbdecoder.cpp
    ${DECODER_DIR}/sortedfiledecoder.cpp
    ${DECODER_DIR}/tdrivedecoder.cpp
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

set(PERFORMANCE_SRCS
    ${PERFORMANCE_DIR}/memoryreporter.cpp
    ${PERFORMANCE_DIR}/performancelogger.cpp
)

set(PATTERNS_SOURCES
    ${COMMON_SRCS}
    ${DATABASE_SRCS}
    ${DECODER_SRCS}
    ${FLOCK_SRCS}
    ${GPS_SRCS}
    ${PERFORMANCE_SRCS}
    main.cpp
)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

## new Patterns design
set(DATACONNECTORS_DIR ${PROJECT_SRC_DIR}/dataconnectors)
set(DATADECODERS_DIR ${PROJECT_SRC_DIR}/datadecoders)
set(DATALISTENERS_DIR ${PROJECT_SRC_DIR}/datalisteners)
set(DATAMODEL_DIR ${PROJECT_SRC_DIR}/datamodel)
set(DATAPROCESSORS_DIR ${PROJECT_SRC_DIR}/dataprocessors)

set(DATACONNECTORS_SRCS
    ${DATACONNECTORS_DIR}/rawdatastring.cpp
)

set(PATTERNSNG_SOURCES
    ${COMMON_DIR}/configparser.cpp
    ${COMMON_DIR}/utils.cpp
    ${DATACONNECTORS_SRCS}
    ${GPS_DIR}/gpspoint.cpp
    mainng.cpp
)

add_executable(patternsng ${PATTERNSNG_SOURCES})
target_compile_definitions(patternsng PRIVATE NEWDESIGN=1)

target_include_directories(patternsng PRIVATE
    ${DATACONNECTORS_DIR}
    ${DATADECODERS_DIR}
    ${DATALISTENERS_DIR}
    ${DATAMODEL_DIR}
    ${DATAPROCESSORS_DIR}
)
# End

add_executable(patterns ${PATTERNS_SOURCES})
target_link_libraries(patterns ${SQLite3_LIBRARIES})
