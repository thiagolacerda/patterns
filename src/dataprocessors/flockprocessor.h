#pragma once

#include "componentfactory.h"
#include "dataprocessor.h"
#include "diskmanager.h"
#include "flock.h"
#include "flockmanager.h"
#include "flockutils.h"
#include "gridmanager.h"
#include "logger.h"

class GPSPointBuffererListenerData;

class FlockProcessor : public DataProcessor {
public:
    FlockProcessor(const std::unordered_map<std::string, std::string>& parameters)
        : DataProcessor(parameters)
        , m_flockUtils(std::make_shared<FlockUtils>())
        , m_flockManager(m_flockUtils)
    {
        m_gridManager.setGridSize(stod(m_parameters["gridSize"]));
        m_flockManager.setTrajectoriesPerFlock(stoul(m_parameters["trajectoriesPerFlock"]));
        m_flockManager.setFlockLength(stoul(m_parameters["flockLength"]));
        m_flockUtils->setRadius(stod(m_parameters["gridSize"]) / 2);
        m_flockUtils->setFlockLength(m_flockManager.flockLength());
        m_flockUtils->setTrajectoriesPerFlock(m_flockManager.trajectoriesPerFlock());
        auto iter = m_parameters.find("logger");
        if (iter != m_parameters.end())
            m_logger.reset(ComponentFactory<Logger>::getComponent(iter->second, {}));
    }

    virtual ~FlockProcessor() {}

    void processData(const ProcessorData& data) override;

    void onDataProviderFinished() override;

    static FlockProcessor* instance(const std::unordered_map<std::string, std::string>& parameters)
    {
        return new FlockProcessor(parameters);
    }

protected:
    void clusterPointsIntoDisks(const std::shared_ptr<Disk>& disk1, const std::shared_ptr<Disk>& disk2,
        const std::vector<std::shared_ptr<GPSPoint>>& points);
    virtual void onGridsReady();

    std::shared_ptr<FlockUtils> m_flockUtils;
    FlockManager m_flockManager;
    GridManager m_gridManager;
    DiskManager m_diskManager;
    std::vector<Flock> m_flocks;
    std::shared_ptr<Logger> m_logger;
};

