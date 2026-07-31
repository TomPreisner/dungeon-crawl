/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once
#include <stdint.h>

namespace Status {

struct EffectData {
    explicit EffectData(const float amt) : amount(amt) {}
    virtual ~EffectData() = default;

    float amount = 0.f;
};

struct EffectData_Damage_Type : public EffectData {
    EffectData_Damage_Type(const float amt, int32_t type) : EffectData(amt), damage_type(type) {}
    int32_t damage_type;
};

struct EffectData_Heal_Type : public EffectData {
    EffectData_Heal_Type(const float amt, int32_t type) : EffectData(amt), heal_type(type) {}
    int32_t heal_type;
};

} // namespace Status