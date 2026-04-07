/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#include "code/client/modules/common/status_effect_module_common.h"

#include "code/core/log_manager.h"

namespace Module {
CREATE_LOGGER(StatusEffectModule_Common);

bool StatusEffectModule_Common::init_module(const YAML::Node& node) {
    if (!node.IsMap()) {
        return false;
    }

    const YAML::Node& status_name_node = node["status_name"];
    if (status_name_node) {
        if (status_name_node.IsScalar()) {
            try {
                m_status_effect_name = status_name_node.as<std::string>();
            } catch (const YAML::TypedBadConversion<std::string>& e) {
                LOG_ERROR(StatusEffectModule_Common, "Value in \"status_name\" is not a string, skipping: " + YAML::Dump(status_name_node));
            }
        } else {
            LOG_ERROR(StatusEffectModule_Common, "YAML node \"status_name\" is not a scalar, skipping:\n" + YAML::Dump(status_name_node))
        }
    }

    const YAML::Node& status_type_node = node["status_type"];
    if (status_type_node) {
        m_status_effect_type = Status::StatusEffect::convert_to_type(status_type_node);
        if (!m_status_effect_type) {
            LOG_ERROR(StatusEffectModule_Common, "YAML node \"status_type\" is not valid:\n" + YAML::Dump(status_type_node))
        }
    }

    return true;
}

} // namespace Module
