/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include <cstdint>
#include "code/client/messages/proto/heal.pb.h"

namespace Messages {

// This is meant for calls that apply health directly to the 
//  health, bypassing module checks
struct ApplyDirectHeal {
    const float amount;
    code::client::messages::Heal::HealType heal_type;
};

// This is meant for calls that apply damage directly to the 
//  health, bypassing module checks
struct ApplyDirectDamage {
    const float amount;
    const int32_t damage_type; //< This is a bitfield
};

} // namespace Messages
