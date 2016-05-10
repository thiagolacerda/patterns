#pragma once

#include <cmath>
#include <memory>
#include <queue>
#include <set>
#include "datalistener.h"
#include "gpspoint.h"

class DataModel;

struct GPSPointSharedPtrComp {
    bool operator() (const std::shared_ptr<GPSPoint>& lhs, const std::shared_ptr<GPSPoint>& rhs)
    {
        return *lhs < *rhs;
    }
};

class Sorter : public DataListener {
public:
    Sorter(const std::unordered_map<std::string, std::string>& parameters)
        : DataListener(parameters)
        , m_maxInMemory(pow(2, 30) / sizeof(GPSPoint))
        , m_currentFileIndex(0)
    {
        m_finalName = m_parameters.at("finalname");
    }

    void onDataReceived(const DataModel& dataModel) override;

    static Sorter* instance(const std::unordered_map<std::string, std::string>& parameters)
    {
        return new Sorter(parameters);
    }

private:
    void onDataProviderFinished() override;
    void flushPointsToFile();
    void mergeFiles();
    void flushFileToFile(std::ifstream& input, std::ofstream& outpu);
    std::set<std::shared_ptr<GPSPoint>, GPSPointSharedPtrComp> m_points;
    std::queue<std::string> m_temporaryFiles;
    const uint64_t m_maxInMemory;
    uint64_t m_currentFileIndex;
    std::string m_finalName;
};

