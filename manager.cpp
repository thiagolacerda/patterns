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

void Manager::processGPSTuple(const std::tuple<unsigned long, double, double, unsigned long>& tuple)
{
    unsigned long tID;
    double latitude;
    double longitude;
    unsigned long timestamp;
    std::tie(tID, latitude, longitude, timestamp) = tuple;
    unsigned index = timestamp / Config::timeSlotSize();

    if (Config::coordinateSystem() == Config::Cartesian)
        Utils::latLongToMeters(latitude, longitude, &latitude, &longitude);

    if (m_pointsPerTimeSlot.find(index) == m_pointsPerTimeSlot.end())
        m_pointsPerTimeSlot[index] = std::vector<std::shared_ptr<GPSPoint>>();

    m_pointsPerTimeSlot[index].push_back(std::shared_ptr<GPSPoint>(new GPSPoint(latitude, longitude, timestamp, tID)));
}

void Manager::retrieveData()
{
    m_dbDecoder->retrievePoints();
}
