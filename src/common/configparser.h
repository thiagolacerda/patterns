#pragma once

#include <istream>
#include <string>
#include <unordered_map>

class ConfigParser {
public:
    std::unordered_map<std::string, std::string> getSectionConfig(const std::string& section) const;
    std::string getValue(const std::string& section, const std::string& key) const;
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> getConfig() const;

    bool loadConfig(const std::string& path);
    bool loadConfig(std::istream& inputStream);

    void reset() { m_config.clear(); }

private:
    bool isSectionValid(const std::string& section) const;
    bool validateConfig() const;
    std::pair<std::string, std::string> getKeyAndValue(const std::string& configLine) const;

    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> m_config;
};

