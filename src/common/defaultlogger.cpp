#include "defaultlogger.h"

#include <fstream>
#include "componentfactory.h"

void DefaultLogger::initLogger(const std::string& header, const std::string& fileName)
{
    const std::string& outName = fileName.empty() ? "patterns_log.txt" : fileName;
    std::ofstream out(outName, std::ofstream::out);
    out << header << "\n";
}

void DefaultLogger::log(const std::string& msg, const std::string& fileName)
{
    const std::string& outName = fileName.empty() ? "patterns_log.txt" : fileName;
    std::ofstream out(outName, std::ofstream::app);
    out << msg << "\n";
}

REGISTER_LOGGER("default", "d", DefaultLogger::instance);

