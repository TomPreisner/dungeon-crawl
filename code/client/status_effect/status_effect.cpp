/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#include "code/client/status_effect/status_effect.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "code/core/log_manager.h"

namespace Status {
CREATE_LOGGER(StatusEffect);

StatusEffectType convert_to_status_effect_type(std::string str) {
    static std::map<std::string, StatusEffectType> s_string_to_enum = {
        {"FIRE", StatusEffectType::FIRE},
        {"BLEED", StatusEffectType::BLEED},
        {"POISON", StatusEffectType::POISON},
    };

    std::transform(str.begin(), str.end(), str.begin(),
                    [](char c) { return std::toupper(c); });

    if (s_string_to_enum.find(str) != s_string_to_enum.end()) {
        return s_string_to_enum[str];
    }
    
    LOG_ERROR(StatusEffect, "Invalid enum string: " + str);
    return StatusEffectType::NONE;
}

std::optional<StatusEffectType> StatusEffect::convert_to_type(const YAML::Node& node) {
    StatusEffectType type;
    if (!node.IsScalar()) {
        return std::nullopt;
    }
    try {
        type = convert_to_status_effect_type(node.as<std::string>());
    } catch (const YAML::TypedBadConversion<std::string>& e) {
        return std::nullopt;
    }

    if (StatusEffectType::NONE == type) {
        return std::nullopt;
    }

    return type;
}

StatusEffect::StatusEffect() {
    std::map<StatusState, core::State> stateMap = {
    { StatusState::INACTIVE,
      {
        core::State::StateNoOp,
        core::State::StateUpdateNoOp,
        [&]() { return Inactive_OnExit(); }
      }
    },
    { StatusState::ACTIVE,
      {
        [&]() { return Active_OnEnter(); },
        [&](const std::chrono::milliseconds& dt) { return Active_OnUpdate(dt); },
        [&]() { return Active_OnExit(); }
      }
    },
    { StatusState::ENDED,
      {
        [&]() { return Ended_OnEnter(); },
        core::State::StateUpdateNoOp,
        core::State::StateNoOp
      }
    }
    };

    m_state_machine.init(StatusState::INACTIVE, stateMap);

    // Use the boost uuid library to create a uuid to uniquely identify this status effect instance
    boost::uuids::random_generator gen;
    boost::uuids::uuid id = gen();
    m_uuid_string = boost::uuids::to_string(id);
}

StatusEffect::StatusEffect(const YAML::Node& node)
    : StatusEffect()
{
    if (!node.IsMap()) {
        LOG_ERROR(StatusEffect, "Status Effect node is not a map, unable to initialize: " + YAML::Dump(node));
        return;
    }

    const YAML::const_iterator itEnd = node.end();
    for (YAML::const_iterator iter = node.begin(); iter != itEnd; ++iter) {        
        std::string key;
        try {
            key = iter->first.as<std::string>();  
        } catch (const YAML::TypedBadConversion<std::string>& e) {
            // Not a string 
            LOG_ERROR(StatusEffect, "Key in map is not a string, skipping: " + YAML::Dump(iter->first));
            continue;
        }

        if (key == "type") {
            std::optional<StatusEffectType> type = convert_to_type(iter->second);
            if (type.has_value()) {
                m_status_type = type.value();
            } else {
                LOG_ERROR(StatusEffect, "Failed to convert \"type\", skipping: " + YAML::Dump(iter->second));
                m_status_type = StatusEffectType::NONE;
            }
        } else if (key == "update_rate_sec") {
            float seconds = 0.f;
            extract_scalar(iter->second, "update_rate_sec", seconds);
            if (seconds >= 0) {
                m_update_rate_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                                        std::chrono::duration<float>(seconds));
            } else {
                LOG_ERROR(StatusEffect, "Invalid value for \"update_rate_sec\", skipping: " + YAML::Dump(iter->second));
            }
        } else if (key == "duration_sec") {
            float seconds = 0.f;
            extract_scalar(iter->second, "duration_sec", seconds);
            if (seconds >= 0) {
                m_duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                                    std::chrono::duration<float>(seconds));
            } else {
                LOG_ERROR(StatusEffect, "Invalid value for \"duration_sec\", skipping: " + YAML::Dump(iter->second));
            }
        } else if (key == "vfx") {
            // NOTE: TODO: YAML does an implcit type conversion here so more validation is needed
            extract_scalar(iter->second, "vfx", m_vfx);
        } else if (key == "sfx") {
            // NOTE: TODO: YAML does an implcit type conversion here so more validation is needed
            extract_scalar(iter->second, "sfx", m_sfx);
        } else if (key == "icon") {
            // NOTE: TODO: YAML does an implcit type conversion here so more validation is needed
            extract_scalar(iter->second, "icon", m_icon_path);
        } else if (key == "on_heal") {
            extract_effect_sequence(iter->second, "on_heal", m_heal_effects);
        } else if (key == "on_damage") {
            extract_effect_sequence(iter->second, "on_damage", m_damage_effects);
        } else if (key == "on_update") {
            extract_effect_sequence(iter->second, "on_update", m_update_effects);
        } else {
            LOG_ERROR(StatusEffect, "Invalid key: " + key + ", skipping: " + YAML::Dump(iter->second));
        }
    }
}

void StatusEffect::assign_cleanup_callback(std::function<void(const std::string&)> clear_self_from_owner) {
    m_clear_self = clear_self_from_owner;
}

void StatusEffect::extract_effect_sequence(const YAML::Node& node, const std::string& value_key, std::vector<Effect>& value_out) {
    if (!node.IsSequence()) {
        log_error("Key \"" + value_key + "\" is not a sequence, skipping: " + YAML::Dump(node));
        return;
    }

    value_out.clear();
    value_out.reserve(node.size());

    // For each element in the sequence create the effect
    const YAML::const_iterator itEnd = node.end();
    for (YAML::const_iterator iter = node.begin(); iter != itEnd; ++iter) {
        Effect effect(*iter);

        switch (effect.get_effect_type()) {
            case EffectType::NONE:
                break;
            case EffectType::CLEAR:
                effect.RegisterCallback([this](float value) {
                    clear_callback();
                });
                break;
            case EffectType::DAMAGE:
            case EffectType::DAMAGE_PERCENT:
                effect.RegisterCallback([this](float value) {
                    damage_callback(value);
                });
                break;
            case EffectType::HEAL:
            case EffectType::HEAL_PERCENT:
                effect.RegisterCallback([this](float value) {
                    heal_callback(value);
                });
                break;
            case EffectType::MULTIPLY:
                effect.RegisterCallback([this](float value) {
                    augment_callback(value);
                });
                break;
        }

        value_out.push_back(effect);
    }
}

void StatusEffect::log_error(const std::string& message) {
    LOG_ERROR(StatusEffect, message);
}

////////////////////////////////////////////////////////////
// The meat of the Status Effect operation

////////////////////////////////////////////////////////////
void StatusEffect::on_update(const std::chrono::milliseconds& dt) {
    m_state_machine.update_tick(dt);
}

////////////////////////////////////////////////////////////
// These can be called asynchronously outside of the update loop calls
void StatusEffect::on_heal(const float amt) {
    if (!m_is_active) {
        return;
    }
    for (auto & effect : m_heal_effects ) {
        effect.process_effect(amt);
    }
}

void StatusEffect::on_damage(const float amt) {
    if (!m_is_active) {
        return;
    }
    for (auto & effect : m_damage_effects ) {
        effect.process_effect(amt);
    }
}

////////////////////////////////////////////////////////////
// State Machine callbacks
void StatusEffect::Inactive_OnExit() {
    // TODO: play start vfx and sfx? 
}

void StatusEffect::Active_OnEnter() {
    m_is_active = true;
    m_current_tick_ms = std::chrono::milliseconds(0);
    m_last_update_ms = std::chrono::milliseconds(0);
}

void StatusEffect::Active_OnUpdate(const std::chrono::milliseconds& dt) {
    m_current_tick_ms += dt;
    if (m_duration_ms.count() >= 0 &&
        m_current_tick_ms >= m_duration_ms) {
        clear_status_effect();
    } else if (m_update_rate_ms.count() > 0 &&
               (m_current_tick_ms - m_last_update_ms) >= m_update_rate_ms) {
        m_last_update_ms = m_current_tick_ms;
        for (auto & effect : m_update_effects ) {
            effect.process_effect(0.f);
        }
    }
}

void StatusEffect::Active_OnExit() {
    m_is_active = false;
    // TODO: stop start vfx and sfx?  
}

void StatusEffect::Ended_OnEnter() {  
    try {
        m_clear_self(m_uuid_string);
    } catch (const std::bad_function_call& ex) {
        LOG_ERROR(StatusEffect, std::string("Failed to clear status effect with uuid: ") + std::string(ex.what()));
    }
}

////////////////////////////////////////////////////////////

} // namespace Status
