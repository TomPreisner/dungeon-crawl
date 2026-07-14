/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include <stdint.h>
#include <string.h>

#include "code/client/messages/proto/heal.pb.h"

namespace Status {

// This is an abstract class of pure virtual functions. This should have a concrete implementation somewhere,
//  But this is needed to allow the StatusEffect class to not get too closely tied to the StatusEffectManager class.
class StatusEffectCallbackInterface {
public:
    virtual void cleanup_callback(const std::string& status_uuid) = 0;
    virtual void heal_callback(const std::string& status_uuid, const float amount, const code::client::messages::Heal::HealType heal_type) = 0;
    virtual void damage_callback(const std::string& status_uuid, const float amount, const int32_t damage_flags) = 0;
    virtual void augment_callback(const std::string& status_uuid, const float amount) = 0; // TODO, a work in progress of what needs to be sent
};
 
} // namespace Status
