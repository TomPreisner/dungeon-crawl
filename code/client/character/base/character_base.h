/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once
//  The CharacterBase class exist to provide a common base interface 
//      for the enemies, player characters, and non-player characters.

#include <chrono>

#include "behaviortree_cpp/bt_factory.h"
#include "code/client/object/object_dynamic.h"

enum class CharacterType {
    ENEMY,
    NEUTRAL,
    PLAYER,
    PLAYER_ALLY,
};

class CharacterBase : public ObjectDynamic {
private:
    class CharacterBasePasskey {
     private:
        CharacterBasePasskey() = default;
        ~CharacterBasePasskey() = default;
    };

public:
    CharacterType get_type() const { return m_type; }
    
    virtual void on_heal(const int amt) = 0;
    virtual void on_damage(const int amt) = 0;

protected:
    CharacterBase(CharacterType type) : m_type(type) {}
    
    virtual void on_update(const std::chrono::milliseconds& dt) override;

private:
    CharacterType m_type;
    int m_health_amt;
    int m_armor_amt;

    BT::Tree m_behavior_tree;
};
