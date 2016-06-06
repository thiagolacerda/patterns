#pragma once

#include <string>

class Logger {
public:
    virtual void initLogger(const std::string& header, const std::string& fileName = "") = 0;
    virtual void log(const std::string& msg, const std::string& fileName = "") = 0;

    virtual ~Logger() {}
};

