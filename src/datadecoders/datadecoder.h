#pragma once

#include <unordered_map>
#include "datahandler.h"
#include "datalistener.h"

class DataModel;
class RawData;

class DataDecoder : public DataHandler {
public:
    DataDecoder(const std::unordered_map<std::string, std::string>& parameters)
        : m_parameters(parameters)
    {
    }

    virtual void decodeData(const RawData& data) = 0;

    void registerHandler(const std::shared_ptr<DataListener>& listener) { m_handlers.push_back(listener); }

    void sendDataToListeners(const DataModel& dataModel)
    {
        for (const auto& listener : m_handlers)
            std::static_pointer_cast<DataListener>(listener)->onDataReceived(dataModel);
    }

    void onDataProviderFinished() override { endDataHandler(); }

protected:
    std::unordered_map<std::string, std::string> m_parameters;
};

