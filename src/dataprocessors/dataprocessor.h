#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include "datahandler.h"

class ProcessorData;

class DataProcessor : public DataHandler {
public:
    DataProcessor(const std::unordered_map<std::string, std::string>& parameters)
        : m_parameters(parameters)
    {
    }

    virtual void processData(const ProcessorData& data) = 0;

    void registerHandler(const std::shared_ptr<DataHandler>& handler) { m_handlers.push_back(handler); }

protected:
    std::unordered_map<std::string, std::string> m_parameters;
};

