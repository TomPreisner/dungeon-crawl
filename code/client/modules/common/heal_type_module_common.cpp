/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#include "code/client/modules/common/heal_type_module_common.h"

#include "code/core/log_manager.h"

namespace Module {
CREATE_LOGGER(HealTypeModule_Common);

bool HealTypeModule_Common::init_module(const YAML::Node& node) {
    if (!node.IsMap()) {
        return false;
    }

    const YAML::Node& heal_type_node = node["heal_type"];
    std::string heal_type;
    try {
        heal_type = heal_type_node.as<std::string>();
    } catch (const YAML::TypedBadConversion<std::string>& e) {
        LOG_ERROR(HealTypeModule_Common, "Value in \"heal_type\" is not a string, skipping: " + YAML::Dump(heal_type_node));
        return false;
    }
    Heal::HealType value;
    if (Heal::HealType_Parse(heal_type, &value)) {
        m_heal_type = value;
    } else {
        LOG_ERROR(HealTypeModule_Common, "Invalid heal type: " + heal_type)
        return false;
    }
    
    const YAML::Node& amount_node = node["amount"];
    try {
        m_amount = amount_node.as<float>();
    } catch (const YAML::TypedBadConversion<float>& e) {
        LOG_ERROR(HealTypeModule_Common, "Value in \"amount\" is not a float, skipping: " + YAML::Dump(amount_node));
        return false;
    }

    return true;
}
} // namespace Module
