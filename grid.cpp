#include "grid.h"

#include <iostream>
#include "gpspoint.h"

void Grid::addPoint(GPSPoint* point)
{
    m_points.push_back(point);
}

void Grid::dump()
{
    std::cout << "Number of points in grid: " << m_points.size() << std::endl;
    for (auto iter = m_points.begin(); iter != m_points.end(); ++iter) {
        std::cout << "\t";
        (*iter)->dump();
    }
}
