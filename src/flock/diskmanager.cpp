#include "diskmanager.h"

#include <math.h>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include "config.h"
#include "disk.h"
#include "gpspoint.h"
#include "utils.h"

bool DiskManager::tryInsertDisk(Disk* disk)
{
    for (auto iter = m_disks.begin(); iter != m_disks.end();) {
        unsigned count = (*iter)->countIntersection(disk);
        if (disk->numberOfTrajectories() == count)  // disk is a subset of iter->second, do not insert
            return false;

        if ((*iter)->numberOfTrajectories() != count)
            ++iter;  // Different disks... do nothing
        else
            iter = m_disks.erase(iter);  // disk is a superset of iter->second
    }

    m_disks.push_back(disk);
    return true;
}

void DiskManager::computeDisks(GPSPoint* point1, GPSPoint* point2, uint32_t timestamp, Disk** disk1, Disk** disk2)
{
    if (Utils::fuzzyEqual(point1->latitudeMeters(), point2->latitudeMeters()) &&
        Utils::fuzzyEqual(point1->longitudeMeters(), point2->longitudeMeters()))
        return;

    if (Config::isInCompatibilityMode())
        getDisksPaperVersion(point1, point2, timestamp, disk1, disk2);
    else
        getDisks(point1, point2, timestamp, disk1, disk2);
}

void DiskManager::getDisks(GPSPoint* point1, GPSPoint* point2, uint32_t timestamp, Disk** disk1, Disk** disk2)
{
    double midX;
    double midY;
    double vectorX;
    double vectorY;
    double perpNormVectorX;
    double perpNormVectorY;
    double x1 = point1->longitudeMeters();
    double y1 = point1->latitudeMeters();
    double x2 = point2->longitudeMeters();
    double y2 = point2->latitudeMeters();
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

// This code was copied from the paper as is! (changed only some names of variables)
void DiskManager::getDisksPaperVersion(GPSPoint* point1, GPSPoint* point2, uint32_t timestamp,
    Disk** disk1, Disk** disk2)
{
    double x1 = point1->trajectoryId() < point2->trajectoryId() ? point1->longitudeMeters() : point2->longitudeMeters();
    double y1 = point1->trajectoryId() < point2->trajectoryId() ? point1->latitudeMeters() : point2->latitudeMeters();
    double x2 = point1->trajectoryId() < point2->trajectoryId() ? point2->longitudeMeters() : point1->longitudeMeters();
    double y2 = point1->trajectoryId() < point2->trajectoryId() ? point2->latitudeMeters() : point1->latitudeMeters();
    double x1Squared = x1 * x1;
    double x2Squared = x2 * x2;
    double y1Squared = y1 * y1;
    double y2Squared = y2 * y2;
    double twoY1Y2 = 2.0 * y1 * y2;
    double twoY2MinusY1 = 2.0 * (y2 - y1);
    double tmp = x2Squared - x1Squared - y1Squared + y2Squared;
    double A = (x1 - x2) / (y2 - y1);
    A = A * A + 1.0;
    double B = 2.0 * ((x1 - x2) / (y2 - y1) * ((tmp - twoY1Y2 + 2.0 * y1Squared) / twoY2MinusY1) - x1);
    double C =  x1Squared - Config::radius() * Config::radius() +
            ((tmp - twoY1Y2 + 2.0 * y1Squared) / twoY2MinusY1) *
            ((tmp - twoY1Y2 + 2.0 * y1Squared) / twoY2MinusY1);
    double delta = B * B - 4.0 * A * C;

    if (delta < 0) return;
    if (Utils::fuzzyEqual(A, 0)) return;
    if (Utils::fuzzyEqual(y2, y1)) return;

    // calculating x
    double cX1 = (-B + sqrt(delta)) / (2.0 * A);
    double cX2 = (-B - sqrt(delta)) / (2.0 * A);
    // calculating y
    double cY1 = (2.0 * x1 * cX1 - 2.0 * x2 * cX1 + tmp) / twoY2MinusY1;
    double cY2 = (2.0 * x1 * cX2 - 2.0 * x2 * cX2 + tmp) / twoY2MinusY1;

    *disk1 = new Disk(cX1, cY1, timestamp);
    *disk2 = new Disk(cX2, cY2, timestamp);
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
