/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#include "damage_type_module_common.h"

#include "code/client/damage/proto/damage.pb.h"

#include "code/core/log_manager.h"

namespace Module {
CREATE_LOGGER(DamageTypeModule_Common);

bool DamageTypeModule_Common::init_module(const YAML::Node& node) {
    if (!node.IsMap()) {
        clear_values();
        return false;
    }

    const YAML::Node& damage_type_node = node["damage_type"];
    if (!damage_type_node) {
        LOG_ERROR(DamageTypeModule_Common, "\"damage_type\" is not present, skipping:\n" + YAML::Dump(node));
        clear_values();
        return false;
    }

    if (!damage_type_node.IsSequence()) {
        LOG_ERROR(DamageTypeModule_Common, "\"damage_type\" is not a sequence, skipping:\n" + YAML::Dump(damage_type_node));
        clear_values();
        return false;
    }
    
    for (int i = 0; i < damage_type_node.size(); ++i) {
        std::string damage_type;
        try {
            damage_type = damage_type_node[i].as<std::string>();
        } catch (const YAML::TypedBadConversion<std::string>& e) {
            LOG_ERROR(DamageTypeModule_Common, "A value in \"damage_type\" is not a string, skipping:\n" + YAML::Dump(damage_type_node));
            clear_values();
            return false;
        }
        Damage::DamageType value;
        if (Damage::DamageType_Parse(damage_type, &value)) {
            if (m_damage_type.has_value()) {
                m_damage_type = m_damage_type.value() | value;
            } else {
                m_damage_type = value;
            }
        } else {
            LOG_ERROR(DamageTypeModule_Common, "Invalid damage type: " + damage_type)
            clear_values();
            return false;
        }
    }

    const YAML::Node& amount_node = node["amount"];
    if (!amount_node) {
        LOG_ERROR(DamageTypeModule_Common, "\"amount\" is not present, skipping:\n" + YAML::Dump(node));
        clear_values();
        return false;
    }

    try {
        m_amount = amount_node.as<float>();
    } catch (const YAML::TypedBadConversion<float>& e) {
        LOG_ERROR(DamageTypeModule_Common, "Value in \"amount\" is not a float, skipping:\n" + YAML::Dump(amount_node));
        clear_values();
        return false;
    }

    return true;
}

void DamageTypeModule_Common::clear_values() {
    m_damage_type = std::nullopt;
    m_amount = std::nullopt;
}

} // namespace Module
