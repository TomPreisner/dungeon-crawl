/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include "code/client/damage/proto/damage.pb.h"
#include "code/client/health/proto/heal.pb.h"

namespace Module {

class HealthModule {
public:
    HealthModule() = default;
    virtual ~HealthModule() {}

    virtual void process_heal(Heal& incoming) = 0;
    virtual void process_damage(Damage& incoming) = 0;
};

} // namespace Module
