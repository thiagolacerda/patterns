#include "lifemapdbdecoder.h"

#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <algorithm>
#include <iostream>
#include "sqlitedbmanager.h"

LifeMapDBDecoder::LifeMapDBDecoder(const std::vector<std::string>& parameters)
    : DatabaseDecoder(new SQLiteDBManager())
    , m_path(parameters.empty() ? "" : parameters.at(0))
    , m_dbFilesPrefix(parameters.size() < 2 ? "" : parameters.at(1))
    , m_currentId(0)
    , m_currentDbFileIndex(0)
    , m_selectQuery("SELECT _latitude, _longitude, _time_location FROM locationTable;")
{
    struct dirent *ent;
    struct stat st;
    int dbPrefixLen = m_dbFilesPrefix.length();
    DIR* dir = opendir(m_path.c_str());
    while ((ent = readdir(dir)) != NULL) {
        const std::string fileName = ent->d_name;
        const std::string fullFileName = m_path + "/" + fileName;

        if (fileName[0] == '.' || stat(fullFileName.c_str(), &st) == -1
            || (st.st_mode & S_IFDIR) != 0)
            continue;

        int periodPos = fileName.find_last_of(".");
        if (fileName.substr(periodPos + 1) == "db" && fileName.substr(0, dbPrefixLen) == m_dbFilesPrefix)
            m_dbFiles.push_back(fullFileName);
    }
    closedir(dir);
}

void LifeMapDBDecoder::connectToDB()
{
    if (m_manager->isConnected())
        return;

    m_manager->connect(m_dbFiles.at(m_currentDbFileIndex), "", "");
}

void LifeMapDBDecoder::done()
{
    m_currentDbFileIndex = 0;
    m_currentId = 0;
    DatabaseDecoder::done();
}

void LifeMapDBDecoder::doDecodeRow(void* row)
{
    double latitude;
    double longitude;
    std::string timestampStr;
    for (int col = 0; col < 3; ++col) {
        if (col == 0)
            latitude = m_manager->getColumnAsDouble(row, col) / 1000000;
        else if (col == 1)
            longitude = m_manager->getColumnAsDouble(row, col) / 1000000;
        else
            timestampStr = m_manager->getColumnAsString(row, col);
    }
    // tm.tm_year = year - 1900
    struct tm tm;
    memset(&tm, 0, sizeof(struct tm));
    tm.tm_year = atoi(timestampStr.substr(0, 4).c_str()) - 1900;
    tm.tm_mon = atoi(timestampStr.substr(4, 2).c_str());
    tm.tm_mday = atoi(timestampStr.substr(6, 2).c_str());
    tm.tm_hour = atoi(timestampStr.substr(8, 2).c_str());
    tm.tm_min = atoi(timestampStr.substr(10, 2).c_str());
    tm.tm_sec = atoi(timestampStr.substr(12, 2).c_str());
    uint64_t timestamp = mktime(&tm);

    m_listener(std::make_tuple(m_currentId, latitude, longitude, timestamp));
}

uint64_t LifeMapDBDecoder::doRetrievePoints(int64_t batchSize)
{
    uint64_t retrieved = 0;
    while (m_currentDbFileIndex < m_dbFiles.size() && (batchSize == -1 || retrieved < uint64_t(batchSize))) {
        connectToDB();
        int periodPos = m_dbFiles[m_currentDbFileIndex].find_last_of(".");
        int dbPrefixLen = m_path.size() + m_dbFilesPrefix.size() + 1;
        m_currentId = atoi(m_dbFiles[m_currentDbFileIndex].substr(dbPrefixLen, periodPos - dbPrefixLen).c_str());
        retrieved += m_manager->retrievePoints(m_selectQuery, batchSize);
        if (batchSize == -1 || retrieved < uint64_t(batchSize)) {
            m_manager->disconnect();
            ++m_currentDbFileIndex;
        }
    }
    return retrieved;
}
