/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include "code/client/messages/proto/damage.pb.h"
#include "code/client/messages/proto/heal.pb.h"

namespace Module {

class HealthModule {
public:
    HealthModule() = default;
    virtual ~HealthModule() {}

    virtual void process_heal(code::client::messages::Heal& incoming) {}
    virtual void process_damage(code::client::messages::Damage& incoming) {}
};

} // namespace Module
