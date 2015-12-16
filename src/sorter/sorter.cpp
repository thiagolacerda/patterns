#include "sorter.h"

#include <fstream>
#include <sstream>
#include "databasedecoder.h"
#include "factory.h"
#include "sortedfiledecoder.h"

void Sorter::processGPSTuple(const std::tuple<uint32_t, double, double, uint64_t>& gpsTuple)
{
    m_points.insert(GPSPoint(std::get<1>(gpsTuple), std::get<2>(gpsTuple),
        std::get<3>(gpsTuple), std::get<0>(gpsTuple)));
}

int64_t Sorter::fetchPoints(int64_t batchSize, const std::string& fileName, DatabaseDecoder* decoder)
{
    int64_t fetched = decoder->retrievePoints(batchSize);
    std::ofstream out(fileName, std::ofstream::out);
    for (const GPSPoint& point : m_points)
        printPointToFile(point, out);

    out.close();
    m_points.clear();
    return fetched;
}

void Sorter::sortDB()
{
    DatabaseDecoder* dbDecoder = Factory::dbDecoderInstance(Config::decoder());
    if (!dbDecoder)
        return;

    dbDecoder->setListenerFunction(std::bind(&Sorter::processGPSTuple, this, std::placeholders::_1));
    m_numberOfRecords = dbDecoder->numberOfRecords();
    if (m_numberOfRecords <= m_maxInMemory) {
        fetchPoints(-1, dbDecoder->decoderName(), dbDecoder);
    } else {
        uint64_t counter = 0;
        while (m_processedRecords < m_numberOfRecords) {
            std::ostringstream oss;
            oss << "tmp." << dbDecoder->decoderName() << "." << counter;
            const std::string& fileName = oss.str();
            m_processedRecords += fetchPoints(m_maxInMemory, fileName, dbDecoder);
            m_temporaryFiles.push(fileName);
            ++counter;
        }
        mergeFiles(dbDecoder->decoderName());
    }
    dbDecoder->done();
    delete dbDecoder;
}

void Sorter::mergeFiles(const std::string& finalFileName)
{
    SortedFileDecoder* decoder1 = static_cast<SortedFileDecoder*>(Factory::dbDecoderInstance(Config::SortedFile));
    SortedFileDecoder* decoder2 = static_cast<SortedFileDecoder*>(Factory::dbDecoderInstance(Config::SortedFile));
    GPSPoint point1;
    GPSPoint point2;
    std::function<void (const std::tuple<uint32_t, double, double, uint64_t>&)> point1Func =
        [&point1](const std::tuple<uint32_t, double, double, uint64_t>& gpsTuple) mutable
        {
            point1 = GPSPoint(std::get<1>(gpsTuple), std::get<2>(gpsTuple),
                std::get<3>(gpsTuple), std::get<0>(gpsTuple));
        };

    std::function<void (const std::tuple<uint32_t, double, double, uint64_t>&)> point2Func =
        [&point2](const std::tuple<uint32_t, double, double, uint64_t>& gpsTuple) mutable
        {
            point2 = GPSPoint(std::get<1>(gpsTuple), std::get<2>(gpsTuple),
                std::get<3>(gpsTuple), std::get<0>(gpsTuple));
        };

    uint64_t counter = 0;
    while (m_temporaryFiles.size() > 1) {
        const std::string& file1Name = m_temporaryFiles.front();
        m_temporaryFiles.pop();
        const std::string& file2Name = m_temporaryFiles.front();
        m_temporaryFiles.pop();

        decoder1->setPath(file1Name);
        decoder1->setListenerFunction(point1Func);
        decoder2->setPath(file2Name);
        decoder2->setListenerFunction(point2Func);

        std::ostringstream oss;
        oss << "merge." << counter;
        const std::string& mergedFileName = oss.str();
        std::ofstream mergedFile(mergedFileName, std::ofstream::out);

        uint32_t retrieved1 = decoder1->retrievePoints(1);
        uint32_t retrieved2 = decoder2->retrievePoints(1);
        while (retrieved1 && retrieved2) {
            printPointToFile(point1 < point2 ? point1 : point2, mergedFile);

            if (point1 < point2)
                retrieved1 = decoder1->retrievePoints(1);
            else
                retrieved2 = decoder2->retrievePoints(1);
        }

        printPointToFile(retrieved1 ? point1 : point2, mergedFile);

        flushDecoderToFile(retrieved1 ? decoder1 : decoder2, mergedFile);
        mergedFile.close();

        m_temporaryFiles.push(mergedFileName);
        decoder1->done();
        decoder2->done();
        std::remove(file1Name.c_str()); // delete fileName1
        std::remove(file2Name.c_str()); // delete fileName2
        ++counter;
    }
    std::rename(m_temporaryFiles.front().c_str(), finalFileName.c_str());
}

void Sorter::flushDecoderToFile(SortedFileDecoder* decoder, std::ofstream& file)
{
    decoder->setListenerFunction([this, &file](const std::tuple<uint32_t, double, double, uint64_t>& gpsTuple)
        {
            GPSPoint p(std::get<1>(gpsTuple), std::get<2>(gpsTuple), std::get<3>(gpsTuple), std::get<0>(gpsTuple));
            printPointToFile(p, file);
        }
    );
    decoder->retrievePoints();
}

void Sorter::printPointToFile(const GPSPoint& point, std::ofstream& file)
{
    file << point.trajectoryId() << ";" << point.latitude() << ";" <<
        point.longitude() << ";" << point.timestamp() << "\n";
}

