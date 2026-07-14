/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#include "code/client/modules/base/base_module.h"

namespace Module {

BaseModule::BaseModule(const std::string& module_name, const YAML::Node& data) : 
    m_module_name(module_name) {
}

void BaseModule::assign_descriptor(std::shared_ptr<BaseDescriptor> desc) {
    m_base_descriptor = desc;
}

} // namespace Module
