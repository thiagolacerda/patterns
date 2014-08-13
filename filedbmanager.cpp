#include "filedbmanager.h"

#include <stdlib.h>
#include <string>
#include "databasedecoder.h"
#include "patterns.h"

bool FileDBManager::connect(const std::string& address, const std::string& user, const std::string& password)
{
    UNUSED(user);
    UNUSED(password);

    m_file.open(address, std::ifstream::in);
    return m_file.is_open();
}

void FileDBManager::disconnect()
{
    if (m_file.is_open())
        m_file.close();
}

void FileDBManager::retrievePoints(const std::string& query)
{
    UNUSED(query);

    std::string line;
    while (std::getline(m_file, line))
        m_decoder->decodeRow(&line);
}

std::string FileDBManager::getColumnValue(const std::string& row, int colIndex)
{
    unsigned j = 0;
    for (int i = 0; i < colIndex && j < row.length(); ++i) {
        j = row.find_first_of(';', j);
        ++j;
    }

    if (j == row.length())
        return "";

    return row.substr(j, row.find_first_of(';', j) - j);
}

double FileDBManager::getColumnAsDouble(void* row, int colIndex)
{
    std::string* str = (std::string*) row;
    return atof(getColumnValue(*str, colIndex).c_str());
}

unsigned long FileDBManager::getColumnAsUnsignedLong(void* row, int colIndex)
{
    std::string* str = (std::string*) row;
    return strtoul(getColumnValue(*str, colIndex).c_str(), NULL, 10);
}

std::string FileDBManager::getColumnAsString(void* row, int colIndex)
{
    std::string* str = (std::string*) row;
    return getColumnValue(*str, colIndex);
}
