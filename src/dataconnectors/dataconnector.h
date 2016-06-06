#pragma once

#include <unordered_map>
#include <vector>
#include "datadecoder.h"
#include "datahandler.h"

class RawData;

class DataConnector : public DataHandler {
public:
    DataConnector(const std::unordered_map<std::string, std::string>& parameters)
        : m_parameters(parameters)
    {
    }

    ~DataConnector() override {}

    void registerHandler(const std::shared_ptr<DataDecoder>& decoder) { m_handlers.push_back(decoder); }

    virtual bool connect() = 0;
    virtual void disconnect() = 0;
    virtual bool isConnected() = 0;
    virtual void retrieveData() = 0;

    void sendToDecoders(const RawData& data)
    {
        for (auto& decoder : m_handlers)
            std::static_pointer_cast<DataDecoder>(decoder)->decodeData(data);
    }

protected:
    std::unordered_map<std::string, std::string> m_parameters;
};

