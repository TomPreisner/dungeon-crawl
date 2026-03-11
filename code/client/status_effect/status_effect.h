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
    StatusEffect(); // < needed for std container types
    explicit StatusEffect(const YAML::Node& node);
    virtual ~StatusEffect() {}

    virtual void on_heal(const float amt);
    virtual void on_damage(const float amt);
    virtual void on_update(); //< The tick rate is once per turn

    virtual StatusEffectType get_type() const { return m_status_type; }
    virtual std::string get_icon() const { return m_icon_path; }

    virtual void activate() { m_state_machine.request_transition(StatusState::ACTIVE); }
    virtual void clear_status_effect() { m_state_machine.request_transition(StatusState::ENDED); }

protected:
    virtual void clear_callback() { clear_status_effect(); }
    virtual void heal_callback(float amount) { /* TODO: HEAL CHARACTER */ }
    virtual void damage_callback(float amount) { /* TODO: DAMAGE CHARACTER */ }
    virtual void augment_callback(float amount) { /* TODO: AUGMENT VALUE CHARACTER */ }

private:
    // Functions to extract values from the data.
    //  Note: templates were specifically not used for the sake of the logging system being contained
    // only in the source file. Otherwise it is in the header and becomes much more visible. The logging
    // catches bad data. If this grows past 2 functions, it should be revisited.
    void extract_scalar(const YAML::Node& node, const std::string& value_key, std::string& value_out);
    void extract_scalar(const YAML::Node& node, const std::string& value_key, int& value_out);
    void extract_effect_sequence(const YAML::Node& node, const std::string& value_key, std::vector<Effect>& value_out);

    // state functions
    void Inactive_OnExit();
    void Active_OnEnter();
    void Active_OnUpdate(const std::chrono::milliseconds& dt);
    void Active_OnExit();
    void Ended_OnEnter();

    // member variables
    core::StateMachine<StatusState> m_state_machine;

    StatusEffectType m_status_type = StatusEffectType::NONE;
    int m_update_rate_turns = -1;
    int m_duration_turns = -1;
    int m_current_tick = -1;

    std::vector<Effect> m_heal_effects;
    std::vector<Effect> m_damage_effects;
    std::vector<Effect> m_update_effects;

    std::string m_vfx;
    std::string m_sfx;
    std::string m_icon_path;
};

} // namespace Status
