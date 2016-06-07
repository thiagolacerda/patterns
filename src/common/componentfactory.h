#pragma once

#include <functional>
#include <string>
#include <unordered_map>

template<class T>
class ComponentFactory {
public:
    using ComponentCreateFunc = std::function<T* (const std::unordered_map<std::string, std::string>&)>;
    static void registerComponent(const std::string& name, const std::string& shortName, ComponentCreateFunc creator)
    {
        ComponentFactory<T>::m_registry[name] = creator;
        ComponentFactory<T>::m_shortNameMap[shortName] = name;
    }

    static T* getComponent(const std::string& name, const std::unordered_map<std::string, std::string>& parameters)
    {
        return ComponentFactory<T>::m_registry[name](parameters);
    }

    static std::string getFullName(const std::string& shortName)
    {
        return ComponentFactory<T>::m_shortNameMap[shortName];
    }

private:
    static std::unordered_map<std::string, ComponentCreateFunc> m_registry;
    static std::unordered_map<std::string, std::string> m_shortNameMap;
};

template<class T>
std::unordered_map<std::string, typename ComponentFactory<T>::ComponentCreateFunc> ComponentFactory<T>::m_registry;
template<class T>
std::unordered_map<std::string, std::string> ComponentFactory<T>::m_shortNameMap;

template<class T>
class ComponentRegister {
public:
    ComponentRegister(const std::string& name, const std::string& shortName,
        typename ComponentFactory<T>::ComponentCreateFunc creatorFunc)
    {
        ComponentFactory<T>::registerComponent(name, shortName, creatorFunc);
    }
};

#define REGISTER_DATA_CONNECTOR(CLASSNAME, SHORTNAME, CREATOR_FUNCTION) \
    static ComponentRegister<DataConnector> reg(CLASSNAME, SHORTNAME, CREATOR_FUNCTION);

#define REGISTER_DATA_DECODER(CLASSNAME, SHORTNAME, CREATOR_FUNCTION) \
    static ComponentRegister<DataDecoder> reg(CLASSNAME, SHORTNAME, CREATOR_FUNCTION);

#define REGISTER_DATA_LISTENER(CLASSNAME, SHORTNAME, CREATOR_FUNCTION) \
    static ComponentRegister<DataListener> reg(CLASSNAME, SHORTNAME, CREATOR_FUNCTION);

#define REGISTER_DATA_PROCESSOR(CLASSNAME, SHORTNAME, CREATOR_FUNCTION) \
    static ComponentRegister<DataProcessor> reg(CLASSNAME, SHORTNAME, CREATOR_FUNCTION);

#define REGISTER_LOGGER(CLASSNAME, SHORTNAME, CREATOR_FUNCTION) \
    static ComponentRegister<Logger> reg(CLASSNAME, SHORTNAME, CREATOR_FUNCTION);

