#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include "config.h"
#include "manager.h"

int main(int argc, char** argv)
{
    if (argc < 3) {
        std::cerr << "Error: you must provide the grid size (meters), the decoder type and "
            << "(opitionally) the decoder parameters" << std::endl;
        return -1;
    }

    Config::setGridSize(atof(argv[1]));
    Config::setDecoder(argv[2]);
    std::vector<std::string> decoderParams;
    for (int i = 3; i < argc; ++i)
        decoderParams.push_back(std::string(argv[i]));
    Config::setDecoderParameters(decoderParams);
    Manager manager;
    manager.start();
    return 0;
}
