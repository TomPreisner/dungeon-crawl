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
            if (!iter->second.IsScalar()) {
                LOG_ERROR(StatusEffect, "Key \"type\" is not a scalar, skipping: " + YAML::Dump(iter->second));
                continue;
            }
            try {
                m_status_type = convert_to_status_effect_type(iter->second.as<std::string>());
            } catch (const YAML::TypedBadConversion<std::string>& e) {
                LOG_ERROR(StatusEffect, "Value in \"type\" is not a string, skipping: " + YAML::Dump(iter->second));
                m_status_type = StatusEffectType::NONE;
            }
        } else if (key == "update_rate_turns") {
            extract_scalar(iter->second, "update_rate_turns", m_update_rate_turns);
        } else if (key == "duration_turns") {
            extract_scalar(iter->second, "duration_turns", m_duration_turns);
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

void StatusEffect::extract_scalar(const YAML::Node& node, const std::string& value_key, int& value_out) {
    int value = 0;
    if (!node.IsScalar()) {
        LOG_ERROR(StatusEffect, "Key \"" + value_key + "\" is not a scalar, skipping: " + YAML::Dump(node));
        return;
    }
    try {
        value = node.as<int>();
    } catch (const YAML::TypedBadConversion<int>& e) {
        LOG_ERROR(StatusEffect, "Value in \"" + value_key + "\" is not an int, skipping: " + YAML::Dump(node));
        return;
    }
    value_out = value;
}

void StatusEffect::extract_scalar(const YAML::Node& node, const std::string& value_key, std::string& value_out) {
    std::string value;
    if (!node.IsScalar()) {
        LOG_ERROR(StatusEffect, "Key \"" + value_key + "\" is not a scalar, skipping: " + YAML::Dump(node));
        return;
    }
    try {
        value = node.as<std::string>();
    } catch (const YAML::TypedBadConversion<std::string>& e) {
        LOG_ERROR(StatusEffect, "Value in \"" + value_key + "\" is not a string, skipping: " + YAML::Dump(node));
        return;
    }
    value_out = value;
}

void StatusEffect::extract_effect_sequence(const YAML::Node& node, const std::string& value_key, std::vector<Effect>& value_out) {
    if (!node.IsSequence()) {
        LOG_ERROR(StatusEffect, "Key \"" + value_key + "\" is not a sequence, skipping: " + YAML::Dump(node));
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

////////////////////////////////////////////////////////////
// The meat of the Status Effect operation

////////////////////////////////////////////////////////////
void StatusEffect::on_update() {
    //< The tick rate is once per turn, so the tick rate doesn't matter
    const std::chrono::milliseconds tick_rate(1);
    m_state_machine.update_tick(tick_rate);
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
    m_current_tick = 0;
}

void StatusEffect::Active_OnUpdate(const std::chrono::milliseconds& dt) {    
    if (m_duration_turns >= 0 &&
        m_current_tick >= m_duration_turns) {
        clear_status_effect();
    } else if (m_update_rate_turns > 0 &&
               m_current_tick % m_update_rate_turns == 0) {
        for (auto & effect : m_update_effects ) {
            effect.process_effect(0.f);
        }
    }
    ++m_current_tick;
}

void StatusEffect::Active_OnExit() {
    m_is_active = false;
    // TODO: stop start vfx and sfx?  
}

void StatusEffect::Ended_OnEnter() {  
    // TODO: Request clear fom the status manager.
}

////////////////////////////////////////////////////////////

} // namespace Status
