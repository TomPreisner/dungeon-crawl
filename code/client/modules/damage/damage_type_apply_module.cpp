/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/

#include "damage_type_apply_module.h"

#include "code/core/log_manager.h"

namespace Module {
CREATE_LOGGER(DamageTypeApply_Module);

bool DamageTypeApply_Module::init_module(const YAML::Node& node) {
    if (!DamageTypeModule_Common::init_module(node)) {
        return false;
    }

    if (!m_amount.has_value() || m_amount.value() < 0.f) {
        LOG_ERROR(DamageTypeApply_Module, "Amount is invalid: " + std::to_string(m_amount.value_or(-0.001f)));
        clear_values();
        return false;
    }
    return true;
}

void DamageTypeApply_Module::apply_damage(Damage& outgoing) {
    if (!m_damage_type.has_value() || !m_amount.has_value()) {        
        LOG_WARN(DamageTypeApply_Module, "Invalid values in " + get_module_name())
        return; //< do nothing
    }

    outgoing.set_damage_type(outgoing.damage_type() | m_damage_type.value());
    outgoing.set_amount(outgoing.amount() + m_amount.value());
}
} // namespace Module
