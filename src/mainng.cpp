#include <iostream>
#include "orchestrator.h"

void printMessages(const std::vector<std::string>& messages, const std::string& prefix)
{
    for (const auto& message : messages)
        std::cerr << prefix << ": " << message << std::endl;
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cerr << "You should provide a configuration file path" << std::endl;
        return 1;
    }

    Orchestrator orchestrator(argv[1]);
    if (!orchestrator.loadConfig())
        printMessages(orchestrator.warningMessages(), "warning");

    if (!orchestrator.start()) {
        printMessages(orchestrator.errorMessages(), "error");
        return 3;
    }

    return 0;
}
