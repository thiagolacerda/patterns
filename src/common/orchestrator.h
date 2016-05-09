#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "configparser.h"

class Orchestrator {
public:
    Orchestrator(std::string&& configPath)
        : m_configPath(configPath)
    {
    }

    bool start();

    bool loadConfig();
    std::vector<std::string> errorMessages() { return m_errorMessages; }
    std::vector<std::string> warningMessages() { return m_warningMessages; }

private:
    void checkEmptyComponentSet(const std::unordered_set<std::string>& components, const std::string& message);
    template<class S, class T>
    void registerComponents(const std::unordered_map<std::string, std::shared_ptr<S>>& source,
        const std::string& bindKey, const std::unordered_map<std::string, std::shared_ptr<T>>& target);
    std::unordered_set<std::string> getList(const std::string& value);
    template<class T>
    std::unordered_map<std::string, std::shared_ptr<T>> getComponents(const std::unordered_set<std::string>&
        componentNames);

    std::string m_configPath;
    bool warningsCheck();

    ConfigParser m_parser;
    std::vector<std::string> m_errorMessages;
    std::vector<std::string> m_warningMessages;

    std::unordered_set<std::string> m_connectorNames;
    std::unordered_set<std::string> m_decoderNames;
    std::unordered_set<std::string> m_listenerNames;
    std::unordered_set<std::string> m_processorNames;
};

