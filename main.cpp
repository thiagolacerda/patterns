#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include "config.h"
#include "manager.h"

int main(int argc, char** argv)
{
    if (argc < 4) {
        std::cerr << "Error: you must run as follows:" << std::endl;
        std::cerr << "./patterns <grid_size_in_meters> <time_slot_size_in_seconds> <decoder_name> [list_of_decoder_parameters]" << std::endl;
        return -1;
    }

    Config::setGridSize(atof(argv[1]));
    Config::setTimeSlotSize(atof(argv[2]));
    Config::setDecoder(argv[3]);
    std::vector<std::string> decoderParams;
    for (int i = 4; i < argc; ++i)
        decoderParams.push_back(std::string(argv[i]));
    Config::setDecoderParameters(decoderParams);
    Manager manager;
    manager.start();
    return 0;
}
