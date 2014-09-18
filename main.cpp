#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include "config.h"
#include "manager.h"

void dumpParameters()
{
    std::cout << "******** Parameters ********" << std::endl;
    std::cout << "* Number of trajectories per flock: " << Config::numberOfTrajectoriesPerFlock() << std::endl;
    std::cout << "* Length of flocks (seconds): " << Config::flockLength() << std::endl;
    std::cout << "* Grid size (meters): " << Config::gridSize() << std::endl;
    std::cout << "* Time slot size (seconds): " << Config::timeSlotSize() << std::endl;
    std::cout << "* Used Coordinate System: " << Config::coordinateSystemName(Config::coordinateSystem()) << std::endl;
}

int main(int argc, char** argv)
{
    if (argc < 7) {
        std::cerr << "Error: you must run as follows:" << std::endl;
        std::cerr << "./patterns <number_of_trajectories_per_flock> <flock_length_in_seconds> " <<
            "<grid_size_in_meters> <time_slot_size_in_seconds> <decoder_name> <coordinate_system_code> " <<
            "[list_of_decoder_parameters]" << std::endl;
        return -1;
    }

    Config::setNumberOfTrajectoriesPerFlock(atoi(argv[1]));
    Config::setFlockLength(atoi(argv[2]));
    Config::setGridSize(atof(argv[3]));
    Config::setTimeSlotSize(atof(argv[4]));
    Config::setDecoder(argv[5]);
    Config::setCoordinateSystem(Config::CoordinateSystem(atoi(argv[6])));
    dumpParameters();
    std::vector<std::string> decoderParams;
    for (int i = 7; i < argc; ++i)
        decoderParams.push_back(std::string(argv[i]));
    Config::setDecoderParameters(decoderParams);
    Manager manager;
    manager.start();
    return 0;
}
