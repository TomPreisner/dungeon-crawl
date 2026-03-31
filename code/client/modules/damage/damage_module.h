/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include "code/client/damage/proto/damage.pb.h"

namespace Module {

class DamageModule {
public:
    DamageModule() = default;
    virtual ~DamageModule() {}

    virtual void apply_damage(Damage& outgoing) = 0;

private:
    int8_t m_priority = 255;
};

} // namespace Module
