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
    if (!return_val->init_module(data)) {
        LOG_WARN(ModuleFactory, "Failed to initialize module with name: " + name + " using:\n" + YAML::Dump(data));
    }

    return return_val;
}

// This returns a list of names of the registered modules. There are more efficient ways
//    to do this than creating the list every time it is requested, BUT
//    the only current use case is for testing. So there is no reason to store that
//    vector of strings when we don't need it for most of the cases.
//  If this changes, store the list of names at the same time the modules are registered
//    and simply return that instead.
std::vector<std::string> ModuleFactory::list_modules() const {
    std::vector<std::string> return_val;
    return_val.reserve(m_module_registry.size());

    const auto end = m_module_registry.cend();
    for (auto iter = m_module_registry.begin(); iter != end; ++iter) {
        return_val.push_back(iter->first);
    }

    return return_val;
}

} // namespace Module
