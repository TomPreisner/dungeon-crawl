/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#include "heal_type_module_common.h"

#include "code/core/log_manager.h"

namespace Module {
CREATE_LOGGER(HealTypeModule_Common);

bool HealTypeModule_Common::init_module(const YAML::Node& node) {
    if (!node.IsMap()) {
        clear_values();
        return false;
    }

    const YAML::Node& heal_type_node = node["heal_type"];    
    if (!heal_type_node) {
        LOG_ERROR(HealTypeModule_Common, "\"heal_type\" is not present, skipping:\n" + YAML::Dump(node));
        clear_values();
        return false;
    }

    std::string heal_type;
    try {
        heal_type = heal_type_node.as<std::string>();
    } catch (const YAML::TypedBadConversion<std::string>& e) {
        LOG_ERROR(HealTypeModule_Common, "Value in \"heal_type\" is not a string, skipping:\n" + YAML::Dump(heal_type_node));
        clear_values();
        return false;
    }
    Heal::HealType value;
    if (Heal::HealType_Parse(heal_type, &value)) {
        m_heal_type = value;
    } else {
        LOG_ERROR(HealTypeModule_Common, "Invalid heal type: " + heal_type)
        clear_values();
        return false;
    }
    
    const YAML::Node& amount_node = node["amount"];
    if (!amount_node) {
        LOG_ERROR(HealTypeModule_Common, "\"amount\" is not present, skipping:\n" + YAML::Dump(node));
        clear_values();
        return false;
    }
    try {
        m_amount = amount_node.as<float>();
    } catch (const YAML::TypedBadConversion<float>& e) {
        LOG_ERROR(HealTypeModule_Common, "Value in \"amount\" is not a float, skipping:\n" + YAML::Dump(amount_node));
        clear_values();
        return false;
    }

    return true;
}

void HealTypeModule_Common::clear_values() {
    m_heal_type = std::nullopt;
    m_amount = std::nullopt;
}

} // namespace Module
