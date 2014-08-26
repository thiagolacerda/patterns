#include "manager.h"

#include <iostream>
#include "databasedecoder.h"
#include "factory.h"
#include "gpspoint.h"
#include "trajectory.h"

void Manager::start()
{
    m_dbDecoder = Factory::dbDecoderInstance();
    if (!m_dbDecoder)
        return;

    m_dbDecoder->setGPSTupleListener(this);
    retrieveData();
}

void Manager::processGPSTuple(const std::tuple<long, double, double, unsigned long>& tuple)
{
    long tID;
    double latitude;
    double longitude;
    unsigned long timestamp;
    std::tie(tID, latitude, longitude, timestamp) = tuple;
    unsigned index = timestamp / Config::timeSlotSize();

    if (Config::coordinateSystem() == Config::Cartesian)
        Utils::latLongToMeters(latitude, longitude, &latitude, &longitude);

    Trajectory* trajectory = m_trajectoryManager.trajectoryById(tID);
    if (!trajectory) {
        trajectory = new Trajectory(tID);
        m_trajectoryManager.addTrajectory(trajectory);
    }

    if (m_pointsPerTimeSlot.find(index) == m_pointsPerTimeSlot.end())
        m_pointsPerTimeSlot[index] = std::vector<GPSPoint*>();

    m_pointsPerTimeSlot[index].push_back(new GPSPoint(latitude, longitude, timestamp, trajectory));
}

void Manager::retrieveData()
{
    m_dbDecoder->retrievePoints();
}
