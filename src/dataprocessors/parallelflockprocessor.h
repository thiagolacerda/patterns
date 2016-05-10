#pragma once

#include "flockprocessor.h"
#include "flockprocessorthread.h"
#include "sharedbuffer.h"

class ParallelFlockProcessor : public FlockProcessor {
public:
    ParallelFlockProcessor(const std::unordered_map<std::string, std::string>& parameters)
        : FlockProcessor(parameters)
    {
        m_numThreads = stoul(m_parameters.at("threads"));
        for (unsigned i = 0; i < m_numThreads; ++i)
            m_futures.push_back(new FlockProcessorThread(m_buffer, m_flockUtils));
    }

    ~ParallelFlockProcessor() override;

    static ParallelFlockProcessor* instance(const std::unordered_map<std::string, std::string>& parameters)
    {
        return new ParallelFlockProcessor(parameters);
    }

private:
    void onGridsReady() override;

    uint32_t m_numThreads;
    std::vector<FlockProcessorThread*> m_futures;
    GridBuffer m_buffer;
};

