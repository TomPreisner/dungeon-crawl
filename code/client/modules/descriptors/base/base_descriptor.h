/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include <string>
#include "yaml-cpp/yaml.h"

namespace Module {

class BaseDescriptor {
public:
    BaseDescriptor() = default;
    virtual ~BaseDescriptor() {}

    virtual bool init_module(const YAML::Node& node) = 0;

protected:
    virtual void clear_values() = 0; // Don't clear the module name so it is easier to track down issues
};

} // namespace Module
