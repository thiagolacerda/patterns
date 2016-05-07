#include "rawdatastring.h"

std::string RawDataString::getColumnValue(int colIndex) const
{
    uint32_t j = 0;
    for (int i = 0; i < colIndex && j < m_data->length(); ++i) {
        j = m_data->find_first_of(m_separator, j);
        ++j;
    }

    if (j == m_data->length())
        return "";

    return m_data->substr(j, m_data->find_first_of(m_separator, j) - j);
}

int RawDataString::getAsInt(int colIndex) const
{
    return stoi(getColumnValue(colIndex));
}

double RawDataString::getAsDouble(int colIndex) const
{
    return stod(getColumnValue(colIndex));
}

uint32_t RawDataString::getAsUInt32_t(int colIndex) const
{
    return stoul(getColumnValue(colIndex), NULL, 10);
}

uint64_t RawDataString::getAsUInt64_t(int colIndex) const
{
    return stoull(getColumnValue(colIndex), NULL, 10);
}

std::string RawDataString::getAsString(int colIndex) const
{
    return getColumnValue(colIndex);
}

