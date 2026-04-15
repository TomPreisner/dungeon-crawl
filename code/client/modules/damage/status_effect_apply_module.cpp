/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/

#include "status_effect_apply_module.h"

#include "code/core/log_manager.h"

namespace Module {
CREATE_LOGGER(StatusEffectApply_Module);

bool StatusEffectApply_Module::init_module(const YAML::Node& node) {
    if (!StatusEffectModule_Common::init_module(node)) {
        return false;
    }
    
    if (!m_status_effect_name.has_value()) {
        LOG_ERROR(StatusEffectApply_Module, "Invalid status effect name in " + get_module_name() + ". This module will not function.")
        clear_values();
        return false;
    }

    return true;
}

void StatusEffectApply_Module::apply_damage(Damage& outgoing) {
    if (!m_status_effect_name.has_value()) {
        LOG_WARN(StatusEffectApply_Module, "Invalid status effect name in " + get_module_name())
        return; //< do nothing
    }

    std::string* value = outgoing.add_status_effect();
    if (value == nullptr) {
        LOG_WARN(StatusEffectApply_Module, "Unable to add status \"" + m_status_effect_name.value() + "\" in " + get_module_name())
        return; //< do nothing
    }
    *value = m_status_effect_name.value();
}
} // namespace Module
