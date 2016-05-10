#include "sorter.h"

#include <fstream>
#include "componentfactory.h"
#include "datamodel.h"
#include "rawdatastring.h"

void Sorter::onDataReceived(const DataModel& dataModel)
{
    m_points.insert(dataModel.get<GPSPoint>());
    if (m_points.size() >= m_maxInMemory)
        flushPointsToFile();
}

void Sorter::mergeFiles()
{
    uint64_t counter = 0;
    while (m_temporaryFiles.size() > 1) {
        const std::string& file1Name = m_temporaryFiles.front();
        m_temporaryFiles.pop();
        const std::string& file2Name = m_temporaryFiles.front();
        m_temporaryFiles.pop();

        std::ifstream input1(file1Name, std::ifstream::in);
        std::ifstream input2(file2Name, std::ifstream::in);

        const std::string& mergedFileName = "merge." + std::to_string(counter);
        std::ofstream mergedFile(mergedFileName, std::ofstream::out);

        std::string line1;
        std::string line2;
        RawDataString data1("", ";");
        RawDataString data2("", ";");
        while (!input1.eof() && !input2.eof()) {

            if (line1.empty()) {
                std::getline(input1, line1);
                data1.setString(line1);
            }

            if (line2.empty()) {
                std::getline(input2, line2);
                data2.setString(line2);
            }

            const uint64_t t1 = data1.getAsUInt64_t(3);
            const uint64_t t2 = data2.getAsUInt64_t(3);
            if (t1 < t2 && !line1.empty()) {
                mergedFile << line1 << "\n";
                line1.clear();
            } else if (!line2.empty()) {
                mergedFile << line2 << "\n";
                line2.clear();
            }
        }

        if (!line1.empty())
            mergedFile << line1 << "\n";
        else if (!line2.empty())
            mergedFile << line2 << "\n";

        flushFileToFile(input1.eof() ? input2 : input1 , mergedFile);
        mergedFile.close();

        m_temporaryFiles.push(mergedFileName);
        std::remove(file1Name.c_str()); // delete fileName1
        std::remove(file2Name.c_str()); // delete fileName2
        ++counter;
    }
    std::rename(m_temporaryFiles.front().c_str(), m_finalName.c_str());
}

void Sorter::onDataProviderFinished()
{
    flushPointsToFile();
    mergeFiles();
}

void Sorter::flushPointsToFile()
{
    const std::string fileName = "tmp.sorter." + std::to_string(m_currentFileIndex);
    std::ofstream out(fileName, std::ofstream::out);
    for (const auto& point : m_points) {
        out << point->trajectoryId() << ";" << point->latitude() << ";" << point->longitude() << ";" <<
            point->timestamp() << "\n";
    }

    m_points.clear();
    ++m_currentFileIndex;
    m_temporaryFiles.push(fileName);
}

void Sorter::flushFileToFile(std::ifstream& input, std::ofstream& output)
{
    std::string line;
    while (std::getline(input, line))
        output << line << "\n";
}

REGISTER_DATA_LISTENER("sorterlistener", "s", Sorter::instance);
