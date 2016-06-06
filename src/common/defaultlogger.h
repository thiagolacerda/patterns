#pragma once

#include <unordered_map>
#include "logger.h"

class DefaultLogger : public Logger {
public:
    void initLogger(const std::string& header, const std::string& fileName = "") override;
    void log(const std::string& msg, const std::string& fileName = "") override;

    ~DefaultLogger() override {}

    static DefaultLogger* instance(const std::unordered_map<std::string, std::string>&)
    {
        return new DefaultLogger();
    }
};

