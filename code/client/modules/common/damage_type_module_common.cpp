/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#include "code/client/modules/common/damage_type_module_common.h"

#include "code/client/damage/proto/damage.pb.h"

#include "code/core/log_manager.h"

namespace Module {
CREATE_LOGGER(DamageTypeModule_Common);

bool DamageTypeModule_Common::init_module(const YAML::Node& node) {
    if (!node.IsMap()) {
        return false;
    }

    // TODO: make this a sequence of damage_types not a scalar, then OR the types together
    const YAML::Node& damage_type_node = node["damage_type"];
    std::string damage_type;
    try {
        damage_type = damage_type_node.as<std::string>();
    } catch (const YAML::TypedBadConversion<std::string>& e) {
        LOG_ERROR(DamageTypeModule_Common, "Value in \"damage_type\" is not a string, skipping: " + YAML::Dump(damage_type_node));
        return false;
    }
    Damage::DamageType value;
    if (Damage::DamageType_Parse(damage_type, &value)) {
        m_damage_type = value;
    } else {
        LOG_ERROR(DamageTypeModule_Common, "Invalid damage type: " + damage_type)
        return false;
    }
    
    const YAML::Node& amount_node = node["amount"];
    try {
        m_amount = amount_node.as<float>();
    } catch (const YAML::TypedBadConversion<float>& e) {
        LOG_ERROR(DamageTypeModule_Common, "Value in \"amount\" is not a float, skipping: " + YAML::Dump(amount_node));
        return false;
    }

    return true;
}
} // namespace Module
