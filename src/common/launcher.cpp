#include "launcher.h"

#include <iostream>
#include "optionparser.h"
#include "orchestrator.h"

namespace {
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
    if (options[OptionFlags::ConfigFile])
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
        if (param.type == OptionType::Required || param.type == OptionType::Optional)
            conf.emplace(std::make_pair(param.longopt, (options[param.index] ? options[param.index].arg : "")));
    }

    return conf;
}

void printMessages(const std::vector<std::string>& messages, const std::string& prefix)
{
    for (const auto& message : messages)
        std::cerr << prefix << ": " << message << std::endl;
}
}

int launch(int argc, char** argv)
{
    const std::vector<option::Descriptor> usage = {
        {OptionFlags::Void, OptionType::None, "", "", option::Arg::None, "Usage: patternsng <required> [options]\n"},
        {OptionFlags::Void, OptionType::None, "", "", option::Arg::None, "Required:"},
        {OptionFlags::ConfigFile, OptionType::Override, "f", "conf", validateRequired,
            "-f, --conf <Path to config file (when provided, no other is required)"},
        {OptionFlags::DataConnector, OptionType::Required, "c", "connector", validateRequired,
            "-c, --connector <DataConnector short name>" },
        {OptionFlags::DataDecoder, OptionType::Required, "d", "decoder", validateRequired,
            "-d, --decoder <DataDecoder short name>" },
        {OptionFlags::DataListener, OptionType::Required, "l", "listener", validateRequired,
            "-l, --listener <DataListener short name>" },
        {OptionFlags::Void, OptionType::None, "", "", option::Arg::None, "\nOptional:"},
        {OptionFlags::DataProcessor, OptionType::Optional, "p", "processor", validateRequired,
            "-p, --processor <DataProcessor short name>" },
        {OptionFlags::ConnParams, OptionType::Optional, "", "cp", validateRequired, "--cp <DataConnector parameters>" },
        {OptionFlags::DecoParams, OptionType::Optional, "", "dp", validateRequired, "--dp <DataDecoder parameters>" },
        {OptionFlags::ListParams, OptionType::Optional, "", "lp", validateRequired, "--lp <DataListener parameters>" },
        {OptionFlags::ProcParams, OptionType::Optional, "", "pp", validateRequired, "--pp <DataProcessor parameters>" },
        {OptionFlags::Help, OptionType::None, "h", "help", option::Arg::None, "Print usage and exit"}
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

    if (options[OptionFlags::Help] || argc == 0 || !validateArgs(options, usage)) {
        option::printUsage(std::cout, usageArray);
        return 2;
    }

    Orchestrator orchestrator;
    bool success = true;
    if (options[OptionFlags::ConfigFile])
        success = orchestrator.loadConfigFromFileAndRegisterComponents(options[OptionFlags::ConfigFile].arg);
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

