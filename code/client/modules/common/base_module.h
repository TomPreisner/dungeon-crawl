/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include <string>
#include "yaml-cpp/yaml.h"

namespace Module {

class BaseModule {
public:
    explicit BaseModule(const std::string& name) : m_module_name(name) {}
    virtual ~BaseModule() {}

    virtual const std::string& get_module_name() const { return m_module_name; }

    virtual bool init_module(const YAML::Node& node) = 0;

protected:
    virtual void clear_values() = 0; // Don't clear the module name so it is easier to track down issues

    std::string m_module_name;
};

} // namespace Module
