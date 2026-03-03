/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include "code/client/character/base/character_base.h"

class NPCBase : public CharacterBase {
public:
    virtual void on_heal(const int amt) final {}
    virtual void on_damage(const int amt) final {}

protected:
    NPCBase() : CharacterBase(CharacterType::NEUTRAL) {}
};
