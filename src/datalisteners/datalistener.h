#pragma once

#include <string>
#include <unordered_map>
#include "datahandler.h"
#include "dataprocessor.h"

class DataModel;
class ProcessorData;

class DataListener : public DataHandler {
public:
    DataListener(const std::unordered_map<std::string, std::string>& parameters)
        : DataHandler()
        , m_parameters(parameters)
    {
    }

    ~DataListener() override {}

    virtual void onDataReceived(const DataModel& datamodel) = 0;

    void registerHandler(const std::shared_ptr<DataProcessor>& processor) { m_handlers.push_back(processor); }

    virtual void sendDataToProcessors(const ProcessorData& data)
    {
        for (const auto& processor : m_handlers)
            std::static_pointer_cast<DataProcessor>(processor)->processData(data);
    }

protected:
    std::unordered_map<std::string, std::string> m_parameters;
};
