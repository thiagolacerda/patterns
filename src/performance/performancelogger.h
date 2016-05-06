#ifndef PERFORMANCELOGGER_H
#define PERFORMANCELOGGER_H

#include <string>

class PerformanceLogger {
public:
    static void initLogger(const std::string& header, const std::string& fileName = "");
    static void log(const std::string& msg, const std::string& fileName = "");
};
#endif // PERFORMANCELOGGER_H
