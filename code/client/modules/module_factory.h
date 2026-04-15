/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include <functional>
#include <map>
#include <memory>

#include "code/client/modules/common/base_module.h"

namespace Module {

#define REGISTER_MODULE(name, class) \
    m_module_registry[#name] = &ModuleFactory::create<class>;

class ModuleFactory final {
private:
    // The passkey pattern is necessary here to use the the unique_ptr without
    //  exposing a constructor that other classes can use to create an instance
    class ModuleFactoryPasskey {
     private:
        friend ModuleFactory;
        ModuleFactoryPasskey() = default;
        ~ModuleFactoryPasskey() = default;
    };

public:
    ModuleFactory(ModuleFactoryPasskey passkey) : ModuleFactory() { }
    ~ModuleFactory() = default;

    static ModuleFactory* get_Instance() {
        static ModuleFactory::ModuleFactoryPasskey s_passkey;
        static std::unique_ptr<ModuleFactory> s_instance = std::make_unique<ModuleFactory>(s_passkey);
        return s_instance.get();
    }

    // Here we are using YAML nodes to create the modules. 
    //  The YAML nodes represent unstructured data, so each module can take different 
    // assignment data.
    std::shared_ptr<BaseModule> create_module(const std::string& name, const YAML::Node& data);

    std::vector<std::string> list_modules() const;

private:
    typedef std::map<std::string, std::function<std::shared_ptr<BaseModule>(const std::string&)>> ModuleRegistry;
    ModuleRegistry m_module_registry;

    template <typename T>
    static std::shared_ptr<BaseModule> create(const std::string& name) {
        return std::make_shared<T>(name);
    }

    inline ModuleFactory();
    ModuleFactory(const ModuleFactory&) = delete;
    ModuleFactory& operator=(const ModuleFactory&) = delete;
};
} // namespace Module

#include "module_registry.inl"
