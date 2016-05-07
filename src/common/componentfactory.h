#pragma once

#include <string>
#include <unordered_map>

template<class T>
class ComponentFactory {
public:
    using ComponentCreateFunc = std::function<T* (const std::unordered_map<std::string, std::string>&)>;
    static void registerComponent(const std::string& key, ComponentCreateFunc creatorFunc)
    {
        ComponentFactory<T>::m_registry[key] = creatorFunc;
    }

    static T* getComponent(const std::string& key, const std::unordered_map<std::string, std::string>& parameters)
    {
        return ComponentFactory<T>::m_registry[key](parameters);
    }

private:
    static std::unordered_map<std::string, ComponentCreateFunc> m_registry;
};

template<class T>
std::unordered_map<std::string, typename ComponentFactory<T>::ComponentCreateFunc> ComponentFactory<T>::m_registry;

template<class T>
class ComponentRegister {
public:
    ComponentRegister(const std::string& key, typename ComponentFactory<T>::ComponentCreateFunc creatorFunc)
    {
        ComponentFactory<T>::registerComponent(key, creatorFunc);
    }
};

#define REGISTER_DATA_CONNECTOR(CLASSNAME, CREATOR_FUNCTION) \
    static ComponentRegister<DataConnector> reg(CLASSNAME, CREATOR_FUNCTION);

#define REGISTER_DATA_DECODER(CLASSNAME, CREATOR_FUNCTION) \
    static ComponentRegister<DataDecoder> reg(CLASSNAME, CREATOR_FUNCTION);

#define REGISTER_DATA_LISTENER(CLASSNAME, CREATOR_FUNCTION) \
    static ComponentRegister<DataListener> reg(CLASSNAME, CREATOR_FUNCTION);

#define REGISTER_DATA_PROCESSOR(CLASSNAME, CREATOR_FUNCTION) \
    static ComponentRegister<DataProcessor> reg(CLASSNAME, CREATOR_FUNCTION);

