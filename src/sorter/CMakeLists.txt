set(SORTER_SOURCES
    ${COMMON_DIR}/config.cpp
    ${COMMON_DIR}/factory.cpp
    ${DATABASE_DIR}/filedbmanager.cpp
    ${DATABASE_DIR}/sqlitedbmanager.cpp
    ${DECODER_DIR}/basegpsfiledecoder.cpp
    ${DECODER_DIR}/lifemapdbdecoder.cpp
    ${DECODER_DIR}/sortedfiledecoder.cpp
    ${DECODER_DIR}/trucksdbdecoder.cpp
    ${DECODER_DIR}/truckspaperdbdecoder.cpp
    ${GPS_DIR}/gpspoint.cpp
    ${COMMON_DIR}/utils.cpp
    sorter.cpp
    main.cpp
)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

add_executable(sorter ${SORTER_SOURCES})
target_link_libraries(sorter ${SQLite3_LIBRARIES})

