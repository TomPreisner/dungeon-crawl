/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once
//  The CharacterBase class exist to provide a common base interface 
//      for the enemies, player characters, and non-player characters.

#include <chrono>
#include <memory>

#include "behaviortree_cpp/bt_factory.h"
#include "code/client/health/health_manager.h"
#include "code/client/object/object_dynamic.h"
#include "code/client/status_effect/status_effect_manager.h"
#include "code/core/message_system/message_switchboard.h"

enum class CharacterType {
    ENEMY,
    NEUTRAL,
    PLAYER,
    PLAYER_ALLY,
};

class CharacterBase : public ObjectDynamic {
public:
    CharacterType get_type() const { return m_type; }
    
    virtual void on_heal(const int amt) = 0;
    virtual void on_damage(const int amt) = 0;

protected:
    CharacterBase(CharacterType type);
    
    virtual void on_update(const std::chrono::milliseconds& dt) override;

private:
    CharacterType m_type;

    BT::Tree m_behavior_tree;
    core::MessageSwitchboard m_switchboard;
    std::shared_ptr<HealthManager> m_health_manager;
    std::shared_ptr<Status::StatusEffectManager> m_status_manager;
};
