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
    init_state_machine();

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
            // NOTE: TODO: YAML does an implicit type conversion here so more validation is needed
            extract_scalar(iter->second, "vfx", m_vfx);
        } else if (key == "sfx") {
            // NOTE: TODO: YAML does an implicit type conversion here so more validation is needed
            extract_scalar(iter->second, "sfx", m_sfx);
        } else if (key == "icon") {
            // NOTE: TODO: YAML does an implicit type conversion here so more validation is needed
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

StatusEffect::StatusEffect(const StatusEffect& other) {
    // due to the function pointer linkages in the effects and the state machine
    //  a copy constructor is needed to point those items to the new object

    m_status_type = other.m_status_type;
    m_update_rate_ms = other.m_update_rate_ms;
    m_duration_ms = other.m_duration_ms;
    // Intentionally disregard current tick and last update
    //    m_current_tick_ms = std::chrono::milliseconds(-1);
    //    m_last_update_ms = std::chrono::milliseconds(-1);

    // The effect lists need to be copied, but all of the effects need to be
    //  reregistered to the new Status Effect Object
    m_heal_effects = other.m_heal_effects;
    for (auto iter = m_heal_effects.begin(); iter != m_heal_effects.end(); ++iter) {
        register_effect(*iter);
    }

    m_damage_effects = other.m_damage_effects;
    for (auto iter = m_damage_effects.begin(); iter != m_damage_effects.end(); ++iter) {
        register_effect(*iter);
    }
    
    m_update_effects = other.m_update_effects;
    for (auto iter = m_update_effects.begin(); iter != m_update_effects.end(); ++iter) {
        register_effect(*iter);
    }

    m_vfx = other.m_vfx;
    m_sfx = other.m_sfx;
    m_icon_path = other.m_icon_path;

    // Use the boost uuid library to create a uuid to uniquely identify this status effect instance
    boost::uuids::random_generator gen;
    boost::uuids::uuid id = gen();
    m_uuid_string = boost::uuids::to_string(id);

    init_state_machine();
}


void StatusEffect::init_state_machine() {
    std::map<StatusState, core::State> stateMap = {
    { StatusState::INACTIVE,
      {
        core::State::StateNoOp,
        core::State::StateUpdateNoOp,
        [this]() { return Inactive_OnExit(); }
      }
    },
    { StatusState::ACTIVE,
      {
        [this]() { return Active_OnEnter(); },
        [this](const std::chrono::milliseconds& dt) { return Active_OnUpdate(dt); },
        [this]() { return Active_OnExit(); }
      }
    },
    { StatusState::ENDED,
      {
        [this]() { return Ended_OnEnter(); },
        core::State::StateUpdateNoOp,
        core::State::StateNoOp
      }
    }
    };

    m_state_machine.init(StatusState::INACTIVE, stateMap);
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
        register_effect(effect);
        value_out.push_back(effect);
    }
}

void StatusEffect::register_effect(Effect& effect) {
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
                damage_callback(value, 0/*const int32_t damage_flags*/);//TODO FIX
            });
            break;
        case EffectType::HEAL:
        case EffectType::HEAL_PERCENT:
            effect.RegisterCallback([this](float value) {
                heal_callback(value, code::client::messages::Heal::POTION/*const code::client::messages::Heal::HealType heal_type*/);//TODO FIX
            });
            break;
        case EffectType::MULTIPLY:
            effect.RegisterCallback([this](float value) {
                augment_callback(value);//here
            });
            break;
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

void StatusEffect::heal_callback(float amount, const code::client::messages::Heal::HealType heal_type) {
    if (m_callback_interface.expired()) {
        LOG_ERROR(StatusEffect, std::string("Failed to process heal callback effect with uuid: ") + m_uuid_string);
    } else {
        std::shared_ptr<StatusEffectCallbackInterface> callback_interface = m_callback_interface.lock();
        callback_interface->heal_callback(m_uuid_string, amount, heal_type);
    }
}

void StatusEffect::damage_callback(float amount, const int32_t damage_flags) {
    if (m_callback_interface.expired()) {
        LOG_ERROR(StatusEffect, std::string("Failed to process damage callback effect with uuid: ") + m_uuid_string);
    } else {
        std::shared_ptr<StatusEffectCallbackInterface> callback_interface = m_callback_interface.lock();
        callback_interface->damage_callback(m_uuid_string, amount, damage_flags);
    }
}

void StatusEffect::augment_callback(float amount) {
    if (m_callback_interface.expired()) {
        LOG_ERROR(StatusEffect, std::string("Failed to process augment callback effect with uuid: ") + m_uuid_string);
    } else {
        std::shared_ptr<StatusEffectCallbackInterface> callback_interface = m_callback_interface.lock();
        callback_interface->augment_callback(m_uuid_string, amount);
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
    if (m_callback_interface.expired()) {
        LOG_ERROR(StatusEffect, std::string("Failed to clear status effect with uuid: ") + m_uuid_string);
    } else {
        std::shared_ptr<StatusEffectCallbackInterface> callback_interface = m_callback_interface.lock();
        callback_interface->cleanup_callback(m_uuid_string);
    }
}

////////////////////////////////////////////////////////////

} // namespace Status
