#include "lifemapdbdecoder.h"

#include <algorithm>
#include <iostream>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include "gpstuplelistener.h"
#include "sqlitedbmanager.h"

LifeMapDBDecoder::LifeMapDBDecoder(const std::vector<std::string>& parameters)
    : DatabaseDecoder(parameters, new SQLiteDBManager())
    , m_path(parameters.at(0))
    , m_dbFilesPrefix(parameters.at(1))
    , m_currentId(0)
{
    m_manager->setDecoder(this);
}

void LifeMapDBDecoder::decodeRow(void* row)
{
    double latitude;
    double longitude;
    std::string timestampStr;
    unsigned long timestamp;
    struct tm tm;
    memset(&tm, 0, sizeof(struct tm));
    for(int col = 0; col < 3; ++col) {
        if (col == 0)
            latitude = m_manager->getColumnAsDouble(row, col) / 1000000;
        else if (col == 1)
            longitude = m_manager->getColumnAsDouble(row, col) / 1000000;
        else
            timestampStr = m_manager->getColumnAsString(row, col);
    }
    // tm.tm_year = year - 1900
    tm.tm_year = atoi(timestampStr.substr(0, 4).c_str()) - 1900;
    tm.tm_mon = atoi(timestampStr.substr(4, 2).c_str());
    tm.tm_mday = atoi(timestampStr.substr(6, 2).c_str());
    tm.tm_hour = atoi(timestampStr.substr(8, 2).c_str());
    tm.tm_min = atoi(timestampStr.substr(10, 2).c_str());
    tm.tm_sec = atoi(timestampStr.substr(12, 2).c_str());
    timestamp = mktime(&tm);

    m_listener->processGPSTuple(std::make_tuple(m_currentId, latitude, longitude, timestamp));
}

void LifeMapDBDecoder::retrievePoints()
{
    DIR *dir;
    struct dirent *ent;
    struct stat st;
    int dbPrefixLen = m_dbFilesPrefix.length();

    dir = opendir(m_path.c_str());
    while ((ent = readdir(dir)) != NULL) {
        const std::string fileName = ent->d_name;
        const std::string fullFileName = m_path + "/" + fileName;

        if (fileName[0] == '.' || stat(fullFileName.c_str(), &st) == -1
            || (st.st_mode & S_IFDIR) != 0)
            continue;

        int periodPos = fileName.find_last_of(".");
        if (fileName.substr(periodPos + 1) == "db"
            && fileName.substr(0, dbPrefixLen) == m_dbFilesPrefix) {
            m_currentId = atoi(fileName.substr(dbPrefixLen, periodPos - dbPrefixLen).c_str());
            std::string selectQuery = "SELECT _latitude, _longitude, _time_location FROM locationTable;";
            m_manager->connect(fullFileName, "", "");
            m_manager->retrievePoints(selectQuery);
            m_manager->disconnect();
        }
    }
    closedir(dir);
}
