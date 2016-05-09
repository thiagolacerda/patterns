#include "orchestrator.h"

#include <sstream>
#include "componentfactory.h"
#include "dataconnector.h"
#include "datadecoder.h"
#include "dataprocessor.h"

void Orchestrator::checkEmptyComponentSet(const std::unordered_set<std::string>& components, const std::string& message)
{
    if (components.empty())
        m_warningMessages.push_back(message);
}

template<class S, class T>
void Orchestrator::registerComponents(const std::unordered_map<std::string, std::shared_ptr<S>>& source,
    const std::string& bindKey, const std::unordered_map<std::string, std::shared_ptr<T>>& target)
{
    for (const auto& sPair : source) {
        const auto& value = m_parser.getValue(sPair.first, bindKey);
        const auto& tPair = target.find(value);
        if (tPair == target.end())
            m_errorMessages.push_back("No component " + value + " was specified in conf file");
        else
            tPair->second->registerHandler(sPair.second);
    }
}

bool Orchestrator::loadConfig()
{
    m_parser.loadConfig(configPath);

    return warningsCheck();
}

bool Orchestrator::warningsCheck()
{
    m_connectorNames = getList(m_parser.getValue("", "dataconnectors"));
    m_decoderNames = getList(m_parser.getValue("", "datadecoders"));
    m_listenerNames = getList(m_parser.getValue("", "datalisteners"));
    m_processorNames = getList(m_parser.getValue("", "dataprocessors"));

    checkEmptyComponentSet(m_connectorNames, "No Data Connectors were specified");
    checkEmptyComponentSet(m_decoderNames, "No Data Decoders were specified");
    checkEmptyComponentSet(m_listenerNames, "No Data Listeners were specified");
    checkEmptyComponentSet(m_processorNames, "No Data Processors were specified");

    return m_warningMessages.empty();
}

bool Orchestrator::start()
{
    const auto& dataConnectors = getComponents<DataConnector>(m_connectorNames);

    const auto& dataDecoders = getComponents<DataDecoder>(m_decoderNames);
    registerComponents<DataDecoder, DataConnector>(dataDecoders, "dataconnector", dataConnectors);

    const auto& dataListeners = getComponents<DataListener>(m_listenerNames);
    registerComponents<DataListener, DataDecoder>(dataListeners, "datadecoder", dataDecoders);

    const auto& dataProcessors = getComponents<DataProcessor>(m_processorNames);
    registerComponents<DataProcessor, DataListener>(dataProcessors, "datalistener", dataListeners);

    if (!m_errorMessages.empty())
        return false;

    for (const auto& connectorPair : dataConnectors)
        connectorPair.second->retrieveData();

    return true;
}

std::unordered_set<std::string> Orchestrator::getList(const std::string& value)
{
    std::istringstream ss(value);
    std::string token;

    std::unordered_set<std::string> result;
    while(std::getline(ss, token, ','))
        result.insert(token);

    return result;
}

template<class T>
std::unordered_map<std::string, std::shared_ptr<T>> Orchestrator::getComponents(const std::unordered_set<std::string>&
    componentNames)
{
    std::unordered_map<std::string, std::shared_ptr<T>> components;

    for (const auto& key : componentNames) {
        T* component = ComponentFactory<T>::getComponent(key, m_parser.getSectionConfig(key));
        if (component)
            components.emplace(std::make_pair(key, std::shared_ptr<T>(component)));
    }

    return components;
}

