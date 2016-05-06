#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>
#include "config.h"
#include "manager.h"

void dumpParameters()
{
    std::cout << "******** Parameters ********" << std::endl;
    std::cout << "* Number of trajectories per flock: " << Config::numberOfTrajectoriesPerFlock() << std::endl;
    std::cout << "* Length of flocks (time slot units): " << Config::flockLength() << std::endl;
    std::cout << "* Grid size (meters): " << Config::gridSize() << std::endl;
    std::cout << "* Time slot size (seconds): " << Config::timeSlotSize() << std::endl;
    std::cout << "* Used Coordinate System: " << Config::coordinateSystemName(Config::coordinateSystem()) << std::endl;
    std::cout << "* Interpolate: " << (Config::interpolate() ? "true" : "false") << std::endl;
    std::cout << "* Outlier speed cutoff: " << (Config::outlierSpeedCutOff() == -1 ? "No" :
        std::to_string(Config::outlierSpeedCutOff())) << std::endl;
    std::cout << "* Online processing: " << (Config::onlineProcessing() ? "true" : "false") << std::endl;
    std::cout << "* Buffering: " << (Config::buffering() ? "true" : "false") << std::endl;
}

int main(int argc, char** argv)
{
    if (argc < 7) {
        std::cerr << "Error: you must run as follows:" << std::endl;
        std::cerr << "./patterns -n <number_of_trajectories_per_flock> -l <flock_length_in_time_slot_units> " <<
            "-g <grid_size_in_meters> -t <time_slot_size_in_seconds> -d <decoder_name> -s <coordinate_system_code> " <<
            "[-c] (to set compatibility mode) [-i] (interpolate points) [-f <outlier_speed_cutoff>] " <<
            "[-r (tell to flush found flocks to file)] [-o (online processing)] [-m (report performance)] " <<
            "[-b (buffering processing (toggles online processing too))] [list_of_decoder_params]" << std::endl;
        return -1;
    }

    clock_t begin = clock();
    int option;
    while ((option = getopt(argc, argv, ":bciormn:l:g:t:d:s:f:")) != -1) {
        switch (option) {
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
        case 'c':
            Config::setCompatibilityMode(true);
            break;
        case 'i':
            Config::setInterpolate(true);
            break;
        case 'f':
            Config::setOutlierSpeedCutOff(atof(optarg));
            break;
        case 'r':
            Config::setFlushFlocksToFile(true);
            break;
        case 'o':
            Config::setOnlineProcessing(true);
            break;
        case 'b':
            Config::setBuffering(true);
            break;
        case 'm':
            Config::setReportPerformance(true);
            break;
        case '?':
            std::cout << "Unknown parameter: " << optopt << std::endl;
            break;
        default:
            if (optopt == 'n' || optopt == 'l' || optopt == 'g' || optopt == 't' ||
                optopt == 'd' || optopt == 's' || optopt == 'f') {
                std::cout << "parameter " << char(optopt) << " requires an argument" << std::endl;
                return -2;
            }

            std::cout << "should not be here, getopt returned " << char(optopt) << std::endl;
            return -3;
        }
    }

    dumpParameters();
    std::vector<std::string> decoderParams;
    for (int i = optind; i < argc; ++i)
        decoderParams.push_back(std::string(argv[i]));

    Config::setDecoderParameters(decoderParams);
    Manager manager;
    manager.start();
    double elapsedTime = double(clock() - begin) / CLOCKS_PER_SEC;
    std::cout << "Total time: " << elapsedTime << std::endl;
    return 0;
}
