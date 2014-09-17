#include "diskmanager.h"

#include <algorithm>
#include <iomanip>
#include <math.h>
#include <sstream>
#include "config.h"
#include "disk.h"
#include "gpspoint.h"
#include "utils.h"

bool DiskManager::tryInsertDisk(Disk* disk)
{
    for (auto iter = m_disks.begin(); iter != m_disks.end();) {
        unsigned count = (*iter)->countIntersection(disk);
        if (disk->numberOfTrajectories() == count) // disk is a subset of iter->second, do not insert
            return false;

        if ((*iter)->numberOfTrajectories() != count)
            ++iter; // Different disks... do nothing
        else
            iter = m_disks.erase(iter); // disk is a superset of iter->second
    }

    m_disks.push_back(disk);
    return true;
}

void DiskManager::computeDisks(GPSPoint* point1, GPSPoint* point2, unsigned long timestamp, Disk** disk1, Disk** disk2)
{
    if (Utils::fuzzyEqual(point1->latitude(), point2->latitude()) &&
        Utils::fuzzyEqual(point1->longitude(), point2->longitude()))
        return;

    double midX;
    double midY;
    double vectorX;
    double vectorY;
    double perpNormVectorX;
    double perpNormVectorY;
    double x1 = point1->latitude();
    double y1 = point1->longitude();
    double x2 = point2->latitude();
    double y2 = point2->longitude();
    Utils::midPoint(x1, y1, x2, y2, &midX, &midY);
    Utils::toVector(x1, y1, x2, y2, &vectorX, &vectorY);
    double pointsDistance = Utils::vectorLength(vectorX, vectorY);
    double powMultiplyParameter = Config::radiusSquared() - ((pointsDistance / 2.0) * (pointsDistance / 2.0));
    if (powMultiplyParameter < 0)
        return;

    Utils::normalizedVector(vectorY, -vectorX, &perpNormVectorX, &perpNormVectorY);
    double multiplyParameter = sqrt(powMultiplyParameter);
    double c1X = midX + multiplyParameter * perpNormVectorX;
    double c1Y = midY + multiplyParameter * perpNormVectorY;
    double c2X = midX - multiplyParameter * perpNormVectorX;
    double c2Y = midY - multiplyParameter * perpNormVectorY;
    *disk1 = new Disk(c1X, c1Y, timestamp);
    *disk2 = new Disk(c2X, c2Y, timestamp);
}

void DiskManager::clear()
{
    std::for_each(m_disks.begin(), m_disks.end(), [](Disk* disk) { delete disk; });
    m_disks.clear();
}

void DiskManager::dump() const
{
    for (Disk* disk : m_disks) {
        disk->dump();
        disk->dumpTrajectories();
    }
}
