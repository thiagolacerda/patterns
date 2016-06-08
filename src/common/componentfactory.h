#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

template<class T>
class ComponentFactory {
public:
    using ComponentCreateFunc = std::function<T* (const std::unordered_map<std::string, std::string>&)>;

    // I know, singletons suck... but for know I'm lazy to think about anything else for this registry-factory design
    static ComponentFactory& instance() {
        // C++11 guarantees that static initialization is thread safe!
        static ComponentFactory factory;
        return factory;
    }

    void registerComponent(const std::string& name, const std::string& shortName, ComponentCreateFunc creator)
    {
        m_registry.emplace(std::make_pair(name, creator));
        m_shortNameMap.emplace(std::make_pair(shortName, name));
    }

    T* getComponent(const std::string& name, const std::unordered_map<std::string, std::string>& parameters)
    {
        try {
            return m_registry.at(name)(parameters);
        } catch (const std::out_of_range& e) {
            std::cerr << "Component " << name << " is not registered" << std::endl;
            throw;
        }
    }

    std::string getFullName(const std::string& shortName)
    {
        try {
            return m_shortNameMap.at(shortName);
        } catch (const std::out_of_range& e) {
            std::cerr << "Short name " << shortName << " is not registered" << std::endl;
            throw;
        }
    }

private:
    ComponentFactory() {}
    std::unordered_map<std::string, ComponentCreateFunc> m_registry;
    std::unordered_map<std::string, std::string> m_shortNameMap;
};

template<class T>
class ComponentRegister {
public:
    ComponentRegister(const std::string& name, const std::string& shortName,
        typename ComponentFactory<T>::ComponentCreateFunc creatorFunc)
    {
        ComponentFactory<T>::instance().registerComponent(name, shortName, creatorFunc);
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

