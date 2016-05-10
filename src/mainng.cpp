#include <iostream>
#include "optionparser.h"
#include "orchestrator.h"

enum OptionFlags {
    DataConnector,
    DataDecoder,
    DataListener,
    DataProcessor,
    ConnParams,
    DecoParams,
    ListParams,
    ProcParams,
    ConfigFile,
    Help,
    Void
};

enum OptionType {
    Required,
    Optional,
    Override,
    None
};

static const std::unordered_map<std::string, std::string> shortNameNameGetter = {
    {"c", "DataConnector"},
    {"d", "DataDecoder"},
    {"l", "DataListener"},
    {"p", "DataProcessor"}
};

std::string getOptStringConcat(const std::string& shortOpt, const std::string& longOpt)
{
    std::string result = shortOpt.empty() ? "" : "-" + shortOpt;
    return result + (!longOpt.empty() ? (!result.empty() ? ", " : "") + std::string("--") + longOpt : "" );
}

option::ArgStatus validateRequired(const option::Option& o, bool msg)
{
    if (o.arg && o.arg[0])
        return option::ARG_OK;

    if (msg) {
        auto iter = shortNameNameGetter.find(o.desc->shortopt);
        const auto& flagsStr = getOptStringConcat(o.desc->shortopt, o.desc->longopt);
        if (iter != shortNameNameGetter.end())
            std::cout << "A non empty " << iter->second << " should be specified with " << flagsStr << std::endl;
        else
            std::cout << flagsStr << " should not be empty" << std::endl;
    }
    return option::ARG_ILLEGAL;
}

bool validateArgs(option::Option* options, const std::vector<option::Descriptor>& params)
{
    if (options[ConfigFile])
        return true;

    for (const auto& param : params) {
        if (param.type == Required && !options[param.index])
            return false;
    }
    return true;
}

std::unordered_map<std::string, std::string> buildConfig(option::Option* options,
    const std::vector<option::Descriptor>& params)
{
    std::unordered_map<std::string, std::string> conf;

    for (const auto& param : params) {
        if (param.type == Required || param.type == Optional)
            conf.emplace(std::make_pair(param.longopt, (options[param.index] ? options[param.index].arg : "")));
    }

    return conf;
}

void printMessages(const std::vector<std::string>& messages, const std::string& prefix)
{
    for (const auto& message : messages)
        std::cerr << prefix << ": " << message << std::endl;
}

int main(int argc, char** argv)
{
    const std::vector<option::Descriptor> usage = {
        {Void, None, "", "", option::Arg::None, "Usage: patternsng <required> [options]\n"},
        {Void, None, "", "", option::Arg::None, "Required:"},
        {ConfigFile, Override, "f", "conf", validateRequired, "-f, --conf <Path to config file (when provided, no other is required)"},
        {DataConnector, Required, "c", "connector", validateRequired, "-c, --connector <DataConnector short name>" },
        {DataDecoder, Required, "d", "decoder", validateRequired, "-d, --decoder <DataDecoder short name>" },
        {DataListener, Required, "l", "listener", validateRequired, "-l, --listener <DataListener short name>" },
        {Void, None, "", "", option::Arg::None, "\nOptional:"},
        {DataProcessor, Optional, "p", "processor", validateRequired, "-p, --processor <DataProcessor short name>" },
        {ConnParams, Optional, "", "cp", validateRequired, "--cp <DataConnector parameters>" },
        {DecoParams, Optional, "", "dp", validateRequired, "--dp <DataDecoder parameters>" },
        {ListParams, Optional, "", "lp", validateRequired, "--lp <DataListener parameters>" },
        {ProcParams, Optional, "", "pp", validateRequired, "--pp <DataProcessor parameters>" },
        {Help, None, "h", "help", option::Arg::None, "Print usage and exit"}
    };
    const option::Descriptor* usageArray = &usage[0];
    argc -= (argc > 0);
    argv += (argc > 0);

    option::Stats stats(usageArray, argc, argv);
    option::Option* options = (option::Option*) calloc(stats.options_max, sizeof(option::Option));
    option::Option* buffer = (option::Option*) calloc(stats.buffer_max, sizeof(option::Option));
    option::Parser parse(usageArray, argc, argv, options, buffer);

    if (parse.error()) {
        std::cerr << "Error parsing options" << std::endl;
        return 1;
    }

    if (options[Help] || argc == 0 || !validateArgs(options, usage)) {
        option::printUsage(std::cout, usageArray);
        return 2;
    }

    Orchestrator orchestrator;
    bool success = true;
    if (options[ConfigFile])
        success = orchestrator.loadConfigFromFileAndRegisterComponents(options[ConfigFile].arg);
    else
        success = orchestrator.loadConfigFromMapAndRegisterComponents(buildConfig(options, usage));

    if (!success) {
        printMessages(orchestrator.warningMessages(), "warning");
        const auto& errorMessages = orchestrator.errorMessages();
        if (!errorMessages.empty()) {
            printMessages(errorMessages, "error");
            return 3;
        }
    }

    orchestrator.start();

    return 0;
}
