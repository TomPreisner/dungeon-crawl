/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include "code/client/character/base/character_base.h"

class EnemyBase : public CharacterBase {
public:
    virtual void on_heal(const int amt) override;
    virtual void on_damage(const int amt) override;

protected:
    EnemyBase() : CharacterBase(CharacterType::ENEMY) {}
};