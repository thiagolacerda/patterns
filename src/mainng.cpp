#include <iostream>
#include "orchestrator.h"

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cerr << "You should provide a configuration file path" << std::endl;
        return 1;
    }

    Orchestrator orchestrator(argv[1]);
    if (!orchestrator.start()) {
        const auto& messages = orchestrator.errorMessages();
        for (const auto& message : messages)
            std::cerr << message << std::endl;

        return 1;
    }

    return 0;
}
