#include "diskmanager.h"

#include <iomanip>
#include <math.h>
#include <sstream>
#include "config.h"
#include "disk.h"
#include "gpspoint.h"
#include "utils.h"

std::string DiskManager::diskKey(double x, double y)
{
    std::ostringstream oss;
    oss << std::setprecision(10) << x << "_" << y;
    return oss.str();
}

std::shared_ptr<Disk> DiskManager::findDisk(double centerX, double centerY)
{
    auto iter = m_disks.find(diskKey(centerX, centerY));
    return iter != m_disks.end() ? iter->second : nullptr;
}

void DiskManager::computeDisks(GPSPoint* point1, GPSPoint* point2, std::shared_ptr<Disk>& disk1,
    std::shared_ptr<Disk>& disk2)
{
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
    Utils::normalizedVector(vectorY, -vectorX, &perpNormVectorX, &perpNormVectorY);
    double multiplyParameter = sqrt(pow(pointsDistance / 2.0, 2.0) + pow(Config::gridSize(), 2.0));
    double c1X = midX + multiplyParameter * perpNormVectorX;
    double c1Y = midY + multiplyParameter * perpNormVectorY;
    double c2X = midX - multiplyParameter * perpNormVectorX;
    double c2Y = midY - multiplyParameter * perpNormVectorY;
    disk1 = findDisk(c1X, c1Y);
    if (!disk1) {
        disk1.reset(new Disk(c1X, c1Y));
        m_disks[diskKey(c1X, c1Y)] = disk1;
    }

    disk2 = findDisk(c2X, c2Y);
    if (!disk2) {
        disk2.reset(new Disk(c2X, c2Y));
        m_disks[diskKey(c2X, c2Y)] = disk2;
    }
}