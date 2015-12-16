#include <iostream>
#include <unistd.h>
#include "config.h"
#include "sorter.h"

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cerr << "Error: you must run as follows:" << std::endl;
        std::cerr << "./patterns -d <decoder_name> [list_of_decoder_parameters]" << std::endl;
        return -1;
    }

    int option;
    while ((option = getopt(argc, argv, "d:")) != -1) {
        switch (option) {
        case 'd':
            Config::setDecoder(optarg);
            break;
        default:
            return -2;
        }
    }

    std::vector<std::string> decoderParams;
    for (int i = optind; i < argc; ++i)
        decoderParams.push_back(std::string(argv[i]));

    Config::setDecoderParameters(decoderParams);

    Sorter sorter;
    sorter.sortDB();
    return 0;
}

