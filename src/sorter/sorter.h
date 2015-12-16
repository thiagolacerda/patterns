#ifndef SORTER_H
#define SORTER_H

#include <math.h>
#include <set>
#include <queue>
#include "gpspoint.h"

class DatabaseDecoder;
class SortedFileDecoder;

class Sorter {
public:
    Sorter()
        : m_maxInMemory(pow(2, 30) / sizeof(GPSPoint))
        , m_numberOfRecords(0)
        , m_processedRecords(0)
    {}

    void processGPSTuple(const std::tuple<uint32_t, double, double, uint64_t>&);

    void sortDB();

private:
    int64_t fetchPoints(int64_t batchSize, const std::string& fileName, DatabaseDecoder* decoder);
    void mergeFiles(const std::string& finalFileName);
    void printPointToFile(const GPSPoint& point, std::ofstream& file);
    void flushDecoderToFile(SortedFileDecoder* decoder, std::ofstream& file);
    std::set<GPSPoint> m_points;
    std::queue<std::string> m_temporaryFiles;
    const uint64_t m_maxInMemory;
    uint64_t m_numberOfRecords;
    uint64_t m_processedRecords;
};

#endif // SORTER_H

