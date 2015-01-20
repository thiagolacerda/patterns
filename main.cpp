#include <iostream>
#include <stdlib.h>
#include <string>
#include <unistd.h>
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
        std::cerr << "./patterns -n <number_of_trajectories_per_flock> -l <flock_length_in_seconds> " <<
            "-g <grid_size_in_meters> -t <time_slot_size_in_seconds> -d <decoder_name> -s <coordinate_system_code> " <<
            "[list_of_decoder_parameters]" << std::endl;
        return -1;
    }

    int option;
    while ((option = getopt(argc, argv, "n:l:g:t:d:s:")) != -1) {
        switch(option) {
        case 'n':
            Config::setNumberOfTrajectoriesPerFlock(atoi(optarg));
            break;
        case 'l':
            Config::setFlockLength(atoi(optarg));
            break;
        case 'g':
            Config::setGridSize(atof(optarg));
            break;
        case 't':
            Config::setTimeSlotSize(atof(optarg));
            break;
        case 'd':
            Config::setDecoder(optarg);
            break;
        case 's':
            Config::setCoordinateSystem(Config::CoordinateSystem(atoi(optarg)));
            break;
        default:
            if (optopt == 'n' || optopt == 'l' || optopt == 'g' || optopt == 't' || optopt == 'd' || optopt == 's')
                return -2;

            std::cout << "Unknown parameter: " << optopt << std::endl;
        }
    }

    dumpParameters();
    std::vector<std::string> decoderParams;
    for (int i = optind; i < argc; ++i)
        decoderParams.push_back(std::string(argv[i]));

    Config::setDecoderParameters(decoderParams);
    Manager manager;
    manager.start();
    return 0;
}
