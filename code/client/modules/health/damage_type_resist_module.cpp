/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/

#include "damage_type_resist_module.h"

#include "code/core/log_manager.h"

namespace Module {
CREATE_LOGGER(DamageTypeResist_Module);

bool DamageTypeResist_Module::init_module(const YAML::Node& node) {
    if (!node.IsMap()) {
        return false;
    }

    const YAML::Node& partialeffect_node = node["partial_effect_amount"];
    if (partialeffect_node) {
        try {
            m_partial_effect_amount= partialeffect_node.as<float>();
        } catch (const YAML::TypedBadConversion<float>& e) {
            LOG_ERROR(DamageTypeResist_Module, "Value in \"partial_effect_amount\" is not a float, skipping: " + YAML::Dump(partialeffect_node));
            return false;
        }
    }
    
    return DamageTypeModule_Common::init_module(node);
}

void DamageTypeResist_Module::process_damage(Damage& incoming) {
    if (!m_damage_type.has_value()) {        
        LOG_ERROR(DamageTypeResist_Module, "Invalid damage type in " + get_module_name())
        return; //< do nothing
    }
    if (!m_amount.has_value()) {        
        LOG_ERROR(DamageTypeResist_Module, "Invalid damage amount in " + get_module_name())
        return; //< do nothing
    }

    if (incoming.damage_type() == (incoming.damage_type() & m_damage_type.value())) {
        incoming.set_amount(std::max(0.f, incoming.amount() - m_amount.value()));
    } else if ((incoming.damage_type() & m_damage_type.value()) != 0 && m_partial_effect_amount.has_value()) {
        // if it isn't a perfect match, there might be a partial effect that is better than nothing
        incoming.set_amount(std::max(0.f, incoming.amount() - m_partial_effect_amount.value()));
    }
}
} // namespace Module
