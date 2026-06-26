/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once
#include <map>
#include <memory>

#include "code/client/modules/health/health_module_base.h"

namespace Module {

#define REGISTER_HEALTH_MODULE(name, class) \
    m_health_module_registry[#name] = &HealthModuleFactory::create<class>;

class HealthModuleFactory final {
private:
    // The passkey pattern is necessary here to use the the unique_ptr without
    //  exposing a constructor that other classes can use to create an instance
    class HealthModuleFactoryPasskey {
     private:
        friend HealthModuleFactory;
        HealthModuleFactoryPasskey() = default;
        ~HealthModuleFactoryPasskey() = default;
    };

public:
    HealthModuleFactory(HealthModuleFactoryPasskey passkey) : HealthModuleFactory() { }
    ~HealthModuleFactory() = default;

    static HealthModuleFactory* get_Instance() {
        static HealthModuleFactory::HealthModuleFactoryPasskey s_passkey;
        static std::unique_ptr<HealthModuleFactory> s_instance = std::make_unique<HealthModuleFactory>(s_passkey);
        return s_instance.get();
    }

    // Here we are using YAML nodes to create the modules. 
    //  The YAML nodes represent unstructured data, so each module can take different 
    // assignment data.
    std::shared_ptr<HealthModule> create_health_module(const std::string& name, const YAML::Node& data);
    std::vector<std::string> list_modules() const;
private:
    typedef std::map<std::string, std::function<std::shared_ptr<HealthModule>(const std::string&, const YAML::Node&)>> ModuleRegistry;
    ModuleRegistry m_health_module_registry;

    template <typename T>
    static std::shared_ptr<HealthModule> create(const std::string& name, const YAML::Node& data) {
        return std::make_shared<T>(name, data);
    }

    inline HealthModuleFactory();
    HealthModuleFactory(const HealthModuleFactory&) = delete;
    HealthModuleFactory& operator=(const HealthModuleFactory&) = delete;

};

} // namespace Module

#include "health_module_registry.inl"
