/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/

#include "damage_type_resist_module.h"

#include "code/core/log_manager.h"

namespace Module {
CREATE_LOGGER(DamageTypeResist_Module);

bool DamageTypeResist_Module::init_module(const YAML::Node& node) {
    if (!DamageTypeModule_Common::init_module(node)) {
        return false;
    }

    if (!m_amount.has_value() || m_amount.value() < 0.f) {
        LOG_ERROR(DamageTypeResist_Module, "amount is invalid: " + std::to_string(m_amount.value_or(-0.001f)));
        clear_values();
        return false;
    }

    const YAML::Node& partialeffect_node = node["partial_effect_amount"];
    if (partialeffect_node) {
        try {
            m_partial_effect_amount = partialeffect_node.as<float>();
        } catch (const YAML::TypedBadConversion<float>& e) {
            LOG_ERROR(DamageTypeResist_Module, "Value in \"partial_effect_amount\" is not a float, skipping: " + YAML::Dump(partialeffect_node));
            clear_values();
            return false;
        }
        
        if (m_partial_effect_amount.value() < 0.f || m_partial_effect_amount.value() > m_amount.value()) {
            LOG_ERROR(DamageTypeResist_Module, "Value in \"partial_effect_amount\" is invalid, skipping: " + std::to_string(m_partial_effect_amount.value()));
            clear_values();
            return false;
        }
    }
    
    return true;
}

void DamageTypeResist_Module::process_damage(Damage& incoming) {
    if (!m_damage_type.has_value() || !m_amount.has_value()) {        
        LOG_ERROR(DamageTypeResist_Module, "Invalid values in " + get_module_name())
        return; //< do nothing
    }

    if (incoming.damage_type() == (incoming.damage_type() & m_damage_type.value())) {
        incoming.set_amount(std::max(0.f, incoming.amount() - m_amount.value()));
    } else if ((incoming.damage_type() & m_damage_type.value()) != 0 && m_partial_effect_amount.has_value()) {
        // if it isn't a perfect match, there might be a partial effect that is better than nothing
        incoming.set_amount(std::max(0.f, incoming.amount() - m_partial_effect_amount.value()));
    }
}

void DamageTypeResist_Module::clear_values() {
    DamageTypeModule_Common::clear_values();
    m_partial_effect_amount = std::nullopt;
}

} // namespace Module
