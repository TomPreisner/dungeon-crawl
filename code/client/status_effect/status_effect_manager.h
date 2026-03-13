/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include <list>

#include "code/client/status_effect/status_effect.h"
#include "yaml-cpp/yaml.h"

namespace Status {

// Each character should have it's own status effect manager to manage the statuses
//  independently.  The Status' are applied through various means and a copy is stored 
// so it can be managed independently
class StatusEffectManager {
protected:
    struct StatusEffectAction {
        enum ActionType{
            APPLY,
            REMOVE,
            REMOVE_ALL
        };

        ActionType type;
        std::string status_id;
        std::string status_uuid;
    };

public:
    StatusEffectManager() {}
    ~StatusEffectManager() {}

    bool apply_status_effect(const std::string& status_id);
    void clear_status_effect(const std::string& status_uuid);
    void clear_all_status_effects();

    void update_manager();

    void apply_heal_effects(const float amount)  {}     // TODO: Fill in
    void apply_damage_effects(const float amount) {}    // TODO: Fill in

protected:
    // Protected functions exposing functionality for unit tests only
    const std::list<StatusEffect>& test_get_status_effects() const { return m_status_effects; } 
    const std::list<StatusEffectAction>& test_get_pending_status_effect_actions() const { return m_pending_status_effect_actions; } 

private:
    void add_status_effect(const std::string& status_id);
    void remove_status_effect(const std::string& status_uuid);
    void remove_all_status_effects();

    //  These actions and status effects will be created and removed
    // regularly, so these are lists to allow for inser and removal easily
    std::list<StatusEffect> m_status_effects;
    std::list<StatusEffectAction> m_pending_status_effect_actions;
};

} // namespace Status
