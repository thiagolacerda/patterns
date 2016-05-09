#include "configparser.h"

#include <fstream>
#include <utility>
#include "utils.h"

std::unordered_map<std::string, std::string> ConfigParser::getSectionConfig(const std::string& section) const
{
    const auto& iter = m_config.find(section);
    if (iter == m_config.end())
        return std::unordered_map<std::string, std::string>();

    return iter->second;
}

std::string ConfigParser::getValue(const std::string& section, const std::string& key) const
{
    const auto& iter = m_config.find(section);
    if (iter == m_config.end())
        return "";

    const auto& iter2 = iter->second.find(key);
    if (iter2 == iter->second.end())
        return "";

    return iter2->second;
}

std::unordered_map<std::string, std::unordered_map<std::string, std::string>> ConfigParser::getConfig() const
{
    return m_config;
}

bool ConfigParser::isSectionValid(const std::string& section) const
{
    return section.front() == '[' && section.back() == ']' && section.size() > 2;
}

std::pair<std::string, std::string> ConfigParser::getKeyAndValue(const std::string& configLine) const
{
    std::size_t found = configLine.find_first_of("=");
    if (found == std::string::npos)
        return std::move(std::make_pair("", ""));

    const auto key = Utils::trim(configLine.substr(0, found));
    const auto value = Utils::trim(configLine.substr(found + 1));
    return std::move(std::make_pair(key, value));
}

bool ConfigParser::validateConfig() const
{
    for (const auto& sectionValues : m_config) {
        if (sectionValues.second.empty())
            return false;
    }
    return true;
}

bool ConfigParser::loadConfig(const std::string& path)
{
    m_config.clear();
    std::ifstream file(path);
    if (!file.is_open())
        return false;

    return loadConfig(file);
}

bool ConfigParser::loadConfig(std::istream& inputStream)
{
    std::string line;
    std::string currentSection = "";
    bool valid = true;
    while (std::getline(inputStream, line) && valid) {
        auto trimmed = Utils::trim(line);
        if (trimmed.empty() || trimmed.front() == '#') // comments start with #
            continue;

        if (isSectionValid(trimmed)) {
            currentSection = trimmed.substr(1, line.size() - 2);
            m_config[currentSection];
        } else {
            auto keyValuePair = getKeyAndValue(trimmed);
            std::string key = std::get<0>(keyValuePair);
            std::string value = std::get<1>(keyValuePair);
            if (key.empty() || value.empty())
                valid = false;
            else
                m_config[currentSection][key] = value;
        }
    }

    if (!valid || !validateConfig()) {
        m_config.clear();
        return false;
    }

    return true;
}
