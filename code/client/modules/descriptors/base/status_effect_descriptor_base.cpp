/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#include "status_effect_descriptor_base.h"
#include "code/client/status_effect/status_effect_library.h"

#include "code/core/log_manager.h"

namespace Module {
CREATE_LOGGER(StatusEffectDescriptor_Base);

bool StatusEffectDescriptor_Base::init_descriptor(const YAML::Node& node) {
    if (!node.IsMap()) {
        clear_values();
        return false;
    }

    const YAML::Node& status_name_node = node["status_name"];
    if (status_name_node) {
        if (status_name_node.IsScalar()) {
            try {
                m_status_effect_name = status_name_node.as<std::string>();
            } catch (const YAML::TypedBadConversion<std::string>& e) {
                LOG_ERROR(StatusEffectDescriptor_Base, "Value in \"status_name\" is not a string, skipping:\n" + YAML::Dump(status_name_node))
                clear_values();
                return false;
            }
        } else {
            LOG_ERROR(StatusEffectDescriptor_Base, "YAML node \"status_name\" is not a scalar, skipping:\n" + YAML::Dump(status_name_node))
            clear_values();
            return false;
        }

        if (!Status::StatusEffectLibrary::get_Instance()->has_status_effect(m_status_effect_name.value())) {
            LOG_ERROR(StatusEffectDescriptor_Base, "Status effect: \"" + m_status_effect_name.value() + "\" does not exist, skipping.")
            clear_values();
            return false;
        }
        
    }

    const YAML::Node& status_type_node = node["status_type"];
    if (status_type_node) {
        m_status_effect_type = Status::StatusEffect::convert_to_type(status_type_node);
        if (!m_status_effect_type) {
            m_status_effect_name = std::nullopt;
            LOG_ERROR(StatusEffectDescriptor_Base, "YAML node \"status_type\" is not valid:\n" + YAML::Dump(status_type_node))
            clear_values();
            return false;
        }
    }

    if (m_status_effect_name.has_value() && m_status_effect_type.has_value()) {
        LOG_ERROR(StatusEffectDescriptor_Base, "YAML node has too many fields:\n" + YAML::Dump(node))
        clear_values();
        return false;
    }
    
    if (!m_status_effect_name.has_value() && !m_status_effect_type.has_value()) {
        LOG_ERROR(StatusEffectDescriptor_Base, "YAML node has no valid fields:\n" + YAML::Dump(node))
        clear_values();
        return false;
    }

    return true;
}

void StatusEffectDescriptor_Base::clear_values() {
    m_status_effect_name = std::nullopt;
    m_status_effect_type = std::nullopt;
}

} // namespace Module
