/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#include "code/client/status_effect/effects/types/effect_heal.h"

#include <algorithm>
#include <cassert>
#include <map>

#include "code/client/messages/proto/heal.pb.h"
#include "code/core/log_manager.h"

namespace Status {
CREATE_LOGGER(Effect_Heal);

Effect_Heal::Effect_Heal(const YAML::Node& node) {
    init(node);

    if (!is_valid()) {
        LOG_ERROR(Effect_Heal, "Node data is not valid, skipping:\n" + YAML::Dump(node));
        clear_values();
        return;
    }
}

void Effect_Heal::init(const YAML::Node& node) {
    assert(node.IsMap());
    Effect_Base::init(node);

    YAML::Node heal_type_node = node["heal_type"];
    if (!heal_type_node) {
        LOG_ERROR(Effect_Heal, "\"heal_type\" is not present, skipping:\n" + YAML::Dump(node));
        clear_values();
        return;
    }

    if (!heal_type_node.IsSequence()) {
        LOG_ERROR(Effect_Heal, "\"heal_type\" is not a sequence, skipping:\n" + YAML::Dump(heal_type_node));
        clear_values();
        return;
    }

    for (int i = 0; i < heal_type_node.size(); ++i) {
        std::string heal_type;
        try {
            heal_type = heal_type_node[i].as<std::string>();
        } catch (const YAML::TypedBadConversion<std::string>& e) {
            LOG_ERROR(Effect_Heal, "A value in \"heal_type\" is not a string, skipping:\n" + YAML::Dump(heal_type_node));
            clear_values();
            return;
        }
        code::client::messages::Heal::HealType value;
        if (code::client::messages::Heal::HealType_Parse(heal_type, &value)) {
            if (m_heal_type >= 0) {
                m_heal_type = m_heal_type.value() | value;
            } else {
                m_heal_type = value;
            }
        } else {
            LOG_ERROR(Effect_Heal, "Invalid heal type: " + heal_type)
            clear_values();
            return;
        }
    }
}

const bool Effect_Heal::is_valid() const {
    return m_is_valid &&
            get_effect_type() == EffectType::HEAL;
}

float Effect_Heal::process_effect(const EffectData& data) {
    if (!is_valid()) {
        LOG_ERROR(Effect_Heal, "Node data is not valid, skipping.");
        return 0.f;
    }

    if (!m_amount.has_value()) {
        LOG_ERROR(Effect_Heal, "Status::Effect_Heal - m_amount is not valid.");
        return 0.f;
    }

    if (!m_heal_type.has_value()) {
        LOG_ERROR(Effect_Heal, "Status::Effect_Heal - m_heal_type is not valid.");
        return 0.f;
    }

    const float effect_value = m_amount.value();

    if (m_callback) {
        m_callback(effect_value);
    }

    return effect_value;
}

void Effect_Heal::clear_values() {
    Effect_Base::clear_values();

    m_heal_type = std::nullopt;
}

} // namespace Status
