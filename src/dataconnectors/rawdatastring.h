#pragma once

#include <memory>
#include <string>
#include "rawdata.h"

class RawDataString : public RawData {
public:
    RawDataString(const std::shared_ptr<std::string>& data, const std::string& separator)
        : m_data(data)
        , m_separator(separator)
    {
    }

    ~RawDataString() override {}

    int getAsInt(int colIndex) const override;
    std::string getAsString(int colIndex) const override;
    double getAsDouble(int colIndex) const override;
    uint32_t getAsUInt32_t(int colIndex) const override;
    uint64_t getAsUInt64_t(int colIndex) const override;

private:
    std::string getColumnValue(int colIndex) const;
    std::shared_ptr<std::string> m_data;
    std::string m_separator;
};

