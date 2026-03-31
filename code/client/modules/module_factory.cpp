/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/

#include "module_factory.h"

#include "code/core/log_manager.h"

namespace Module {
CREATE_LOGGER(ModuleFactory);
    
std::shared_ptr<BaseModule> ModuleFactory::create_module(const std::string& name, const YAML::Node& data) {
    if (m_module_registry.find(name) == m_module_registry.end()) {
        LOG_WARN(ModuleFactory, "Unable to find module with name: " + name)
        return nullptr;
    }

    std::shared_ptr<BaseModule> return_val = m_module_registry[name](name);
    return_val->init_module(data);

    return return_val;
}
} // namespace Module
