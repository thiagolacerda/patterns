#include "fileconnector.h"

#include "componentfactory.h"
#include "rawdatastring.h"

FileConnector::FileConnector(const std::unordered_map<std::string, std::string>& parameters)
    : DataConnector(parameters)
{
    m_filePath = parameters.at("file");
    m_separator = parameters.at("separator");
    convertEscapedCharactersInSeparator();
}

void FileConnector::convertEscapedCharactersInSeparator()
{
    bool wasBackslash = false;
    int j = 0;
    for (unsigned i = 0; i < m_separator.length(); ++i) {
        char c = m_separator[i];
        if (m_separator[i] != '\\') {
            if (wasBackslash) {
                if (m_separator[i] == 't')
                    c = '\t';
                else if (m_separator[i] == 'n')
                    c = '\n';
                else
                    ++j;
                wasBackslash = false;
            }
            m_separator[j] = c;
            ++j;
        } else {
            wasBackslash = true;
        }
    }
    m_separator[j] = '\0';
}

bool FileConnector::connect()
{
    if (isConnected())
        return true;

    m_file.open(m_filePath, std::ifstream::in);
    return m_file.is_open();
}

void FileConnector::disconnect()
{
    if (m_file.is_open())
        m_file.close();
}

bool FileConnector::isConnected()
{
    return m_file.is_open();
}

void FileConnector::retrieveData()
{
    if (connect()) {
        std::string line;
        while (std::getline(m_file, line)) {
            RawDataString data(std::make_shared<std::string>(line), m_separator);
            sendToDecoders(data);
        }
    }

    endDataHandler();
}

REGISTER_DATA_CONNECTOR("fileconnector", FileConnector::instance);

