/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/

#include "damage_type_resist_percent_module.h"

#include "code/core/log_manager.h"

namespace Module {
CREATE_LOGGER(DamageTypeResistPercent_Module);

bool DamageTypeResistPercent_Module::init_module(const YAML::Node& node) {
    if (!DamageTypeResist_Module::init_module(node)) {
        return false;
    }
    
    if (m_amount.value() < 0.f || m_amount.value() > 1.f) {
        LOG_ERROR(DamageTypeResistPercent_Module, "Invalid damage amount value: " + std::to_string(m_amount.value()))
        clear_values();
        return false;
    }
    
    if (m_partial_effect_amount.has_value() && 
        (m_partial_effect_amount.value() < 0.f || m_partial_effect_amount.value() > 1.f)) {
        LOG_ERROR(DamageTypeResistPercent_Module, "Invalid damage partial effect amount value: " + std::to_string(m_partial_effect_amount.value()))
        clear_values();
        return false;
    }

    return true;
}

void DamageTypeResistPercent_Module::process_damage(code::client::messages::Damage& incoming) {
    if (!m_damage_type.has_value() || !m_amount.has_value()) {        
        LOG_WARN(DamageTypeResistPercent_Module, "Invalid values in " + get_module_name())
        return; //< do nothing
    }

    if (incoming.damage_type() == (incoming.damage_type() & m_damage_type.value())) {
        incoming.set_amount(std::max(0.f, incoming.amount() * (1.f - m_amount.value())));
    } else if ((incoming.damage_type() & m_damage_type.value()) != 0 && m_partial_effect_amount.has_value()) {
        // if it isn't a perfect match, there might be a partial effect that is better than nothing
        incoming.set_amount(std::max(0.f, incoming.amount() * (1.f - m_partial_effect_amount.value())));
    }
}
} // namespace Module
