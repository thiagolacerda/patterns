set(SORTER_SOURCES
    ${GPS_DIR}/gpspoint.cpp
    ${COMMON_DIR}/configparser.cpp
    ${COMMON_DIR}/launcher.cpp
    ${COMMON_DIR}/orchestrator.cpp
    ${COMMON_DIR}/utils.cpp
    ${DATACONNECTORS_DIR}/fileconnector.cpp
    ${DATACONNECTORS_DIR}/rawdatastring.cpp
    ${DATADECODERS_SRCS}
    sorter.cpp
    ../mainng.cpp
)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

add_executable(sorter ${SORTER_SOURCES})
target_compile_definitions(sorter PRIVATE NEWDESIGN=1)

target_include_directories(sorter PRIVATE
    ${COMMON_DIR}
    ${DATACONNECTORS_DIR}
    ${DATADECODERS_DIR}
    ${DATALISTENERS_DIR}
    ${DATAMODEL_DIR}
    ${DATAPROCESSORS_DIR}
    ${GPS_DIR}
)

