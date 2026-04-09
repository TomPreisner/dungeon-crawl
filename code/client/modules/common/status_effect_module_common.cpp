/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#include "status_effect_module_common.h"
#include "code/client/status_effect/status_effect_library.h"

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
                LOG_ERROR(StatusEffectModule_Common, "Value in \"status_name\" is not a string, skipping:\n" + YAML::Dump(status_name_node))
                return false;
            }
        } else {
            LOG_ERROR(StatusEffectModule_Common, "YAML node \"status_name\" is not a scalar, skipping:\n" + YAML::Dump(status_name_node))
            return false;
        }

        if (!Status::StatusEffectLibrary::get_Instance()->has_status_effect(m_status_effect_name.value())) {
            LOG_ERROR(StatusEffectModule_Common, "Status effect: \"" + m_status_effect_name.value() + "\" does not exist, skipping.")
            m_status_effect_name = std::nullopt;
            return false;
        }
        
    }

    const YAML::Node& status_type_node = node["status_type"];
    if (status_type_node) {
        m_status_effect_type = Status::StatusEffect::convert_to_type(status_type_node);
        if (!m_status_effect_type) {
            m_status_effect_name = std::nullopt;
            LOG_ERROR(StatusEffectModule_Common, "YAML node \"status_type\" is not valid:\n" + YAML::Dump(status_type_node))
            return false;
        }
    }

    if (m_status_effect_name.has_value() && m_status_effect_type.has_value()) {
        LOG_ERROR(StatusEffectModule_Common, "YAML node has too many fields:\n" + YAML::Dump(node))
        m_status_effect_name = std::nullopt;
        m_status_effect_type = std::nullopt;
        return false;
    }
    
    if (!m_status_effect_name.has_value() && !m_status_effect_type.has_value()) {
        LOG_ERROR(StatusEffectModule_Common, "YAML node has no valid fields:\n" + YAML::Dump(node))
        return false;
    }

    return true;
}

} // namespace Module
