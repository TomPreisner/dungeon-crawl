/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once
#include <chrono>

#include "yaml-cpp/yaml.h"

namespace Status {

enum class StatusState {
    INACTIVE,
    ACTIVE,
    ENDED
};

class BaseStatusEffect {
public:
    explicit BaseStatusEffect(const YAML::Node& node) { m_icon_path = node["icon_path"]; }
    virtual ~BaseStatusEffect() {}

    virtual void on_turn_start();
    virtual void on_heal(const float amt);
    virtual void on_damage(const float amt);
    virtual void on_update(); //< The tick rate is once per turn
    virtual void on_turn_end();

    virtual std::string get_icon() const { return m_icon_path; }

    virtual bool is_active() const { return m_state == StatusState::ACTIVE; }
    virtual bool is_completed() const { return m_state == StatusState::ENDED; }

    virtual void activate() { m_pending_state = StatusState::ACTIVE; }
    virtual void clear_status_effect() { m_pending_state = StatusState::ENDED; }

protected:
    StatusState m_state = StatusState::INACTIVE;
    StatusState m_pending_state = StatusState::INACTIVE;
    std::string m_icon_path;
};

} // namespace Status
