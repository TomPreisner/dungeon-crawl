/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#include "code/client/status_effect/effects/types/effect_damage.h"

#include <algorithm>
#include <cassert>
#include <map>

#include "code/client/messages/proto/damage.pb.h"
#include "code/core/log_manager.h"

namespace Status {
CREATE_LOGGER(Effect_Damage);

Effect_Damage::Effect_Damage(const YAML::Node& node)  {
    init(node);

    if (!is_valid()) {
        LOG_ERROR(Effect_Damage, "Node data is not valid, skipping:\n" + YAML::Dump(node));
        clear_values();
        return;
    }
}

void Effect_Damage::init(const YAML::Node& node) {
    assert(node.IsMap());
    Effect_Base::init(node);

    YAML::Node damage_type_node = node["damage_type"];
    if (!damage_type_node) {
        LOG_ERROR(Effect_Damage, "\"damage_type\" is not present, skipping:\n" + YAML::Dump(node));
        clear_values();
        return;
    }

    if (!damage_type_node.IsSequence()) {
        LOG_ERROR(Effect_Damage, "\"damage_type\" is not a sequence, skipping:\n" + YAML::Dump(damage_type_node));
        clear_values();
        return;
    }

    for (int i = 0; i < damage_type_node.size(); ++i) {
        std::string damage_type;
        try {
            damage_type = damage_type_node[i].as<std::string>();
        } catch (const YAML::TypedBadConversion<std::string>& e) {
            LOG_ERROR(Effect_Damage, "A value in \"damage_type\" is not a string, skipping:\n" + YAML::Dump(damage_type_node));
            clear_values();
            return;
        }
        code::client::messages::Damage::DamageType value;
        if (code::client::messages::Damage::DamageType_Parse(damage_type, &value)) {
            if (m_damage_type >= 0) {
                m_damage_type = m_damage_type.value() | value;
            } else {
                m_damage_type = value;
            }
        } else {
            LOG_ERROR(Effect_Damage, "Invalid damage type: " + damage_type)
            clear_values();
            return;
        }
    }
}

const bool Effect_Damage::is_valid() const {
    return m_is_valid &&
            get_effect_type() == EffectType::DAMAGE;
}

float Effect_Damage::process_effect(const EffectData& data) {
    if (!is_valid()) {
        LOG_ERROR(Effect_Damage, "Node data is not valid, skipping.");
        return 0.f;
    }

    if (!m_amount.has_value()) {
        LOG_ERROR(Effect_Damage, "Status::Effect_Damage - m_amount is not valid.");
        return 0.f;
    }

    if (!m_damage_type.has_value()) {
        LOG_ERROR(Effect_Damage, "Status::Effect_Damage - m_damage_type is not valid.");
        return 0.f;
    }

    const float effect_value = m_amount.value();
    const int32_t flags = m_damage_type.value();

    if (m_callback) {
        m_callback(effect_value, flags);
    }

    return effect_value;
}

void Effect_Damage::clear_values() {
    Effect_Base::clear_values();

    m_damage_type = std::nullopt;
}


} // namespace Status
