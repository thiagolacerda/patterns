#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include "config.h"
#include "manager.h"

int main(int argc, char** argv)
{
    if (argc < 6) {
        std::cerr << "Error: you must run as follows:" << std::endl;
        std::cerr << "./patterns <number_of_trajectories_per_flock> <grid_size_in_meters> " <<
            "<time_slot_size_in_seconds> <decoder_name> <coordinate_system_code> " <<
            "[list_of_decoder_parameters]" << std::endl;
        return -1;
    }

    Config::setNumberOfTrajectoriesPerFlock(atoi(argv[1]));
    Config::setGridSize(atof(argv[2]));
    Config::setTimeSlotSize(atof(argv[3]));
    Config::setDecoder(argv[4]);
    Config::setCoordinateSystem(Config::CoordinateSystem(atoi(argv[5])));
    std::vector<std::string> decoderParams;
    for (int i = 6; i < argc; ++i)
        decoderParams.push_back(std::string(argv[i]));
    Config::setDecoderParameters(decoderParams);
    Manager manager;
    manager.start();
    return 0;
}
