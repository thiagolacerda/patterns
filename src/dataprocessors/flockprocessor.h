#pragma once

#include "dataprocessor.h"
#include "diskmanager.h"
#include "flock.h"
#include "flockmanager.h"
#include "gridmanager.h"

class GPSPointBuffererListenerData;

class FlockProcessor : public DataProcessor {
public:
    FlockProcessor(const std::unordered_map<std::string, std::string>& parameters)
        : DataProcessor(parameters)
    {
        m_gridManager.setGridSize(stod(m_parameters["gridSize"]));
        m_flockManager.setTrajectoriesPerFlock(stoul(m_parameters["trajectoriesPerFlock"]));
        m_flockManager.setFlockLength(stoul(m_parameters["flockLength"]));
        m_diskManager.setRadius(stod(m_parameters["gridSize"]) / 2);
        m_mask = (1 << m_flockManager.flockLength()) - 1;
    }

    void processData(const ProcessorData& data) override;

    void onDataProviderFinished() override;

    static FlockProcessor* instance(const std::unordered_map<std::string, std::string>& parameters)
    {
        return new FlockProcessor(parameters);
    }

private:
    void clusterPointsIntoDisks(Disk* disk1, Disk* disk2, const std::vector<std::shared_ptr<GPSPoint>>& points,
        const GPSPointBuffererListenerData& data);
    bool shouldInsert(uint32_t trajectoryId, const GPSPointBuffererListenerData& data);
    void validateAndTryStoreDisk(Disk* disk);

    FlockManager m_flockManager;
    GridManager m_gridManager;
    DiskManager m_diskManager;
    std::unordered_map<uint32_t, bool> m_shouldInsertCache;
    uint64_t m_mask;
    uint64_t m_bufferMask;
    std::vector<Flock> m_flocks;
};

