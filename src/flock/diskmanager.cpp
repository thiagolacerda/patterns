#include "diskmanager.h"

#include <algorithm>
#include <cmath>
#if !defined(NEWDESIGN)
#include "config.h"
#endif
#include "disk.h"
#include "gpspoint.h"
#include "utils.h"

void DiskManager::tryInsertDisk(const std::shared_ptr<Disk>& disk)
{
    for (auto iter = m_disks.begin(); iter != m_disks.end();) {
        uint32_t count = (*iter)->countIntersection(disk);
        if (disk->numberOfPoints() == count)
            return; // disk is a subset of disk already in disk manager, do not insert

        if ((*iter)->numberOfPoints() != count)
            ++iter; // Different disks... do nothing
        else
            iter = m_disks.erase(iter); // disk is a superset of *iter
    }

    m_disks.push_back(disk);
}

void DiskManager::computeDisks(GPSPoint* point1, GPSPoint* point2, uint64_t timestamp, std::shared_ptr<Disk>& disk1,
    std::shared_ptr<Disk>& disk2)
{
    const double latMeters1 = point1->latitudeMeters();
    const double longMeters1 = point1->longitudeMeters();
    const double latMeters2 = point2->latitudeMeters();
    const double longMeters2 = point2->longitudeMeters();
    if (Utils::fuzzyEqual(latMeters1, latMeters2) && Utils::fuzzyEqual(longMeters1, longMeters2))
        return;
#if !defined(NEWDESIGN)
    if (Config::isInCompatibilityMode())
        getDisksPaperVersion(point1, point2, timestamp, disk1, disk2);
    else
        getDisks(point1, point2, timestamp, disk1, disk2);
}

void DiskManager::getDisks(GPSPoint* point1, GPSPoint* point2, uint64_t timestamp, std::shared_ptr<Disk>& disk1,
    std::shared_ptr<Disk>& disk2)
{
    double latMeters1 = point1->latitudeMeters();
    double longMeters1 = point1->longitudeMeters();
    double latMeters2 = point2->latitudeMeters();
    double longMeters2 = point2->longitudeMeters();
#endif
    double midX;
    double midY;
    double vectorX;
    double vectorY;
    double perpNormVectorX;
    double perpNormVectorY;
    Utils::midPoint(longMeters1, latMeters1, longMeters2, latMeters2, midX, midY);
    Utils::toVector(longMeters1, latMeters1, longMeters2, latMeters2, vectorX, vectorY);
    double pointsDistance = Utils::vectorLength(vectorX, vectorY);
#if defined(NEWDESIGN)
    double powMultiplyParameter = (m_radius * m_radius) - ((pointsDistance / 2.0) * (pointsDistance / 2.0));
#else
    double powMultiplyParameter = Config::radiusSquared() - ((pointsDistance / 2.0) * (pointsDistance / 2.0));
#endif
    if (powMultiplyParameter < 0)
        return;

    Utils::normalizedVector(vectorY, -vectorX, perpNormVectorX, perpNormVectorY);
    double multiplyParameter = std::sqrt(powMultiplyParameter);
    double c1X = midX + multiplyParameter * perpNormVectorX;
    double c1Y = midY + multiplyParameter * perpNormVectorY;
    double c2X = midX - multiplyParameter * perpNormVectorX;
    double c2Y = midY - multiplyParameter * perpNormVectorY;
#if defined(NEWDESIGN)
    disk1.reset(new Disk(m_radius, c1X, c1Y, timestamp, point1->trajectoryId(), point2->trajectoryId()));
    disk2.reset(new Disk(m_radius, c2X, c2Y, timestamp, point1->trajectoryId(), point2->trajectoryId()));
#else
    disk1.reset(new Disk(c1X, c1Y, timestamp, point1->trajectoryId(), point2->trajectoryId()));
    disk2.reset(new Disk(c2X, c2Y, timestamp, point1->trajectoryId(), point2->trajectoryId()));
#endif
}

#if !defined(NEWDESIGN)
// This code was copied from the paper as is! (changed only some names of variables)
void DiskManager::getDisksPaperVersion(GPSPoint* point1, GPSPoint* point2, uint64_t timestamp,
    std::shared_ptr<Disk>& disk1, std::shared_ptr<Disk>& disk2)
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

    if (delta < 0 || Utils::fuzzyEqual(A, 0) || Utils::fuzzyEqual(y2, y1))
        return;

    // calculating x
    double cX1 = (-B + sqrt(delta)) / (2.0 * A);
    double cX2 = (-B - sqrt(delta)) / (2.0 * A);
    // calculating y
    double cY1 = (2.0 * x1 * cX1 - 2.0 * x2 * cX1 + tmp) / twoY2MinusY1;
    double cY2 = (2.0 * x1 * cX2 - 2.0 * x2 * cX2 + tmp) / twoY2MinusY1;

    disk1.reset(new Disk(cX1, cY1, timestamp, point1->trajectoryId(), point2->trajectoryId()));
    disk2.reset(new Disk(cX2, cY2, timestamp, point1->trajectoryId(), point2->trajectoryId()));
}

void DiskManager::dump() const
{
    for (const auto& disk : m_disks) {
        disk->dump();
        disk->dumpPoints();
    }
}
#endif

void DiskManager::clear()
{
    m_disks.clear();
}

