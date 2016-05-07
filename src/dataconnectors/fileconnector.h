#pragma once

#include <fstream>
#include <string>
#include "dataconnector.h"

class FileConnector : public DataConnector {
public:
    FileConnector(const std::unordered_map<std::string, std::string>& parameters);

    ~FileConnector() override {}

    bool connect() override;
    void disconnect() override;
    bool isConnected() override;
    void retrieveData() override;

    void onDataProviderFinished() override {}

private:
    void convertEscapedCharactersInSeparator();

    std::string m_filePath;
    std::string m_separator;
    std::ifstream m_file;
};

