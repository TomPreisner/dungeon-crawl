/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include <chrono>
#include "code/client/status_effect/effect.h"
#include "yaml-cpp/yaml.h"
#include "code/core/state_machine.h"

namespace Status {
enum class StatusEffectType {
    NONE,
    FIRE,
    BLEED,
    POISON
};

class StatusEffect {
private:
    enum class StatusState {
        INACTIVE,
        ACTIVE,
        ENDED
    };

public:
    static std::optional<StatusEffectType> convert_to_type(const YAML::Node& node);

    StatusEffect(); // < needed for std container types
    StatusEffect(const YAML::Node& node);
    virtual ~StatusEffect() {}

    // not technically needed, but will not clean itself up and must be managed actively
    virtual void assign_cleanup_callback(std::function<void(const std::string&)> clear_self_from_owner);

    virtual void on_heal(const float amt);
    virtual void on_damage(const float amt);
    virtual void on_update(const std::chrono::milliseconds& dt);

    virtual StatusEffectType get_type() const { return m_status_type; }
    virtual std::string get_icon() const { return m_icon_path; }
    virtual std::string get_vfx() const { return m_vfx; }
    virtual std::string get_sfx() const { return m_sfx; }

    virtual void activate() { m_state_machine.request_transition(StatusState::ACTIVE); }
    virtual void clear_status_effect() { m_state_machine.request_transition(StatusState::ENDED); }

    virtual std::string get_uuid() const { return m_uuid_string; }

protected:
    virtual void clear_callback() { clear_status_effect(); }
    virtual void heal_callback(float amount) { /* TODO: HEAL CHARACTER */ }
    virtual void damage_callback(float amount) { /* TODO: DAMAGE CHARACTER */ }
    virtual void augment_callback(float amount) { /* TODO: AUGMENT VALUE CHARACTER */ }

protected:
    // Protected functions exposing functionality for unit tests only
    const bool test_has_valid_update_rate_ms() const { return m_update_rate_ms.count() >= 0; }
    const std::chrono::milliseconds test_get_update_rate_ms() const { return m_update_rate_ms; }
    const bool test_has_valid_duration_ms() const { return m_duration_ms.count() >= 0; }
    const std::chrono::milliseconds test_get_duration_ms() const { return m_duration_ms; }
    const std::chrono::milliseconds test_get_current_tick_ms() const { return m_current_tick_ms; }
    const std::vector<Effect>& test_get_heal_effects() const { return m_heal_effects; }
    const std::vector<Effect>& test_get_damage_effects() const { return m_damage_effects; }
    const std::vector<Effect>& test_get_update_effects() const { return m_update_effects; }

private:
    // Functions to extract values from the data.
    template<typename T>
    void extract_scalar(const YAML::Node& node, const std::string& value_key, T& value_out);

    void extract_effect_sequence(const YAML::Node& node, const std::string& value_key, std::vector<Effect>& value_out);

    void log_error(const std::string& message);

    // state functions
    void Inactive_OnExit();
    void Active_OnEnter();
    void Active_OnUpdate(const std::chrono::milliseconds& dt);
    void Active_OnExit();
    void Ended_OnEnter();

    // member variables
    std::function<void(const std::string&)> m_clear_self;
    core::StateMachine<StatusState> m_state_machine;

    StatusEffectType m_status_type = StatusEffectType::NONE;
    std::chrono::milliseconds m_update_rate_ms = std::chrono::milliseconds(-1);
    std::chrono::milliseconds m_duration_ms = std::chrono::milliseconds(-1);
    std::chrono::milliseconds m_current_tick_ms = std::chrono::milliseconds(-1);
    std::chrono::milliseconds m_last_update_ms = std::chrono::milliseconds(-1);

    std::vector<Effect> m_heal_effects;
    std::vector<Effect> m_damage_effects;
    std::vector<Effect> m_update_effects;

    std::string m_vfx;
    std::string m_sfx;
    std::string m_icon_path;

    std::string m_uuid_string;

    bool m_is_active = false;
};

template<typename T>
void StatusEffect::extract_scalar(const YAML::Node& node, const std::string& value_key, T& value_out) {
    T value;
    if (!node.IsScalar()) {
        log_error("Key \"" + value_key + "\" is not a scalar, skipping: " + YAML::Dump(node));
        return;
    }
    try {
        value = node.as<T>();
    } catch (const YAML::TypedBadConversion<T>& e) {
        log_error("Value in \"" + value_key + "\" is not of the correct type, skipping: " + YAML::Dump(node));
        return;
    }
    value_out = value;
}

} // namespace Status
