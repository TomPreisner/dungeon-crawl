/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/

#include "heal_type_increase_module.h"

#include "code/core/log_manager.h"

namespace Module {
CREATE_LOGGER(HealTypeIncrease_Module);

void HealTypeIncrease_Module::process_heal(Heal& incoming) {
    if (!m_heal_type.has_value()) {        
        LOG_ERROR(HealTypeIncrease_Module, "Invalid heal type in " + get_module_name())
        return; //< do nothing
    }
    if (!m_amount.has_value()) {        
        LOG_ERROR(HealTypeIncrease_Module, "Invalid heal amount in " + get_module_name())
        return; //< do nothing
    }

    if (incoming.type() == m_heal_type.value()) {
        incoming.set_amount(incoming.amount() + m_amount.value());
    }
}
} // namespace Module
