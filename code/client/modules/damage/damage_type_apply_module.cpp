/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/

#include "damage_type_apply_module.h"

#include "code/core/log_manager.h"

namespace Module {
CREATE_LOGGER(DamageTypeApply_Module);

void DamageTypeApply_Module::apply_damage(Damage& outgoing) {
    if (!m_damage_type.has_value()) {        
        LOG_ERROR(DamageTypeApply_Module, "Invalid damage type in " + get_module_name())
        return; //< do nothing
    }
    if (!m_amount.has_value()) {        
        LOG_ERROR(DamageTypeApply_Module, "Invalid damage amount in " + get_module_name())
        return; //< do nothing
    }

    outgoing.set_damage_type(outgoing.damage_type() | m_damage_type.value());
    outgoing.set_amount(outgoing.amount() + m_amount.value());
}
} // namespace Module
