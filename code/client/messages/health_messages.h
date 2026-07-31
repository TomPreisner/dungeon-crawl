/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include <cstdint>
#include <string>
#include "code/client/messages/proto/heal.pb.h"

namespace Messages {

// This is meant for calls that apply health directly to the 
//  health, bypassing module checks
struct ApplyDirectHeal {
    const float amount;
    const int32_t heal_type; //< This is a bitfield
    std::string heal_source_uuid;
};

// This is meant for calls that apply damage directly to the 
//  health, bypassing module checks
struct ApplyDirectDamage {
    const float amount;
    const int32_t damage_type; //< This is a bitfield
    std::string damage_source_uuid;
};

struct OnHealthChange {
    const float amount_change;  //< can be positive or negative
    const float current_health;
    std::string health_manager_uuid;
};

struct OnDeath {
    std::string killer_source_uuid;
};

} // namespace Messages
