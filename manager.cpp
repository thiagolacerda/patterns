#include "manager.h"

#include <iostream>
#include "databasedecoder.h"
#include "factory.h"
#include "gpspoint.h"
#include "trajectory.h"

void Manager::start()
{
    m_dbDecoder = Factory::dbDecoderInstance();
    if (m_dbDecoder)
        retrieveData();
}

void Manager::retrieveData()
{
    m_dbDecoder->retrievePoints();
    unsigned long resultsSize = m_dbDecoder->results.size();
    for (unsigned long i = 0; i < resultsSize; ++i) {
        long tID;
        double latitude;
        double longitude;
        unsigned long timestamp;
        std::tie(tID, latitude, longitude, timestamp) = m_dbDecoder->results.at(i);
        Trajectory* trajectory = m_trajectoryManager.trajectoryById(tID);
        if (!trajectory) {
            trajectory = new Trajectory(tID);
            m_trajectoryManager.addTrajectory(trajectory);
        }

        GPSPoint* point = new GPSPoint(latitude, longitude, timestamp, trajectory);
        trajectory->addPoint(point);
        m_gridManager.addPointToGrid(point);
    }
}
