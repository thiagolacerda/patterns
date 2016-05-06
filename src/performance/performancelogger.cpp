#include "performancelogger.h"

#include <fstream>

void PerformanceLogger::initLogger(const std::string& header, const std::string& fileName)
{
    const std::string& outName = fileName.empty() ? "performance_log.txt" : fileName;
    std::ofstream out(outName, std::ofstream::out);
    out << header << "\n";
}

void PerformanceLogger::log(const std::string& msg, const std::string& fileName)
{
    const std::string& outName = fileName.empty() ? "performance_log.txt" : fileName;
    std::ofstream out(outName, std::ofstream::app);
    out << msg << "\n";
}
