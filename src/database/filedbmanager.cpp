#include "filedbmanager.h"

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

uint64_t FileDBManager::retrievePoints(const std::string& query, int64_t batchSize)
{
    UNUSED(query);

    uint64_t retrieved = 0;
    std::string line;
    while ((batchSize == -1 || retrieved < uint64_t(batchSize)) && std::getline(m_file, line)) {
        m_decoder->decodeRow(&line);
        retrieved++;
    }
    return retrieved;
}

std::string FileDBManager::getColumnValue(const std::string& row, int colIndex)
{
    uint32_t j = 0;
    for (int i = 0; i < colIndex && j < row.length(); ++i) {
        j = row.find_first_of(m_separator, j);
        ++j;
    }

    if (j == row.length())
        return "";

    return row.substr(j, row.find_first_of(m_separator, j) - j);
}

double FileDBManager::getColumnAsDouble(void* row, int colIndex)
{
    std::string* str = (std::string*) row;
    return stod(getColumnValue(*str, colIndex));
}

uint32_t FileDBManager::getColumnAsUInt32(void* row, int colIndex)
{
    std::string* str = (std::string*) row;
    return stoul(getColumnValue(*str, colIndex), NULL, 10);
}

uint64_t FileDBManager::getColumnAsUInt64(void* row, int colIndex)
{
    std::string* str = (std::string*) row;
    return stoull(getColumnValue(*str, colIndex), NULL, 10);
}

std::string FileDBManager::getColumnAsString(void* row, int colIndex)
{
    std::string* str = (std::string*) row;
    return getColumnValue(*str, colIndex);
}
