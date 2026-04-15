/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/

#include "heal_type_increase_module.h"

#include "code/core/log_manager.h"

namespace Module {
CREATE_LOGGER(HealTypeIncrease_Module);

bool HealTypeIncrease_Module::init_module(const YAML::Node& node) {
    if (!HealTypeModule_Common::init_module(node)) {
        return false;
    }

    if (!m_amount.has_value() || m_amount.value() < 0.f) {
        LOG_ERROR(HealTypeIncrease_Module, "amount is invalid: " + std::to_string(m_amount.value_or(-0.001f)));
        clear_values();
        return false;
    }

    return true;
}

void HealTypeIncrease_Module::process_heal(Heal& incoming) {
    if (!m_heal_type.has_value() || !m_amount.has_value()) {
        LOG_WARN(HealTypeIncrease_Module, "Invalid values in " + get_module_name());
        return; //< do nothing
    }

    if (incoming.type() == m_heal_type.value()) {
        incoming.set_amount(incoming.amount() + m_amount.value());
    }
}
} // namespace Module
