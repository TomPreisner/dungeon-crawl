/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include <functional>
#include <optional>
#include "yaml-cpp/yaml.h"

#include "code/client/status_effect/effects/types/effect_damage.h"
#include "code/client/status_effect/effects/effect_data.h"

namespace Status {
class EffectFactory;  //< forward declare the friend class

class Effect_Damage_Multiply : public Effect_Damage {
private:
    friend EffectFactory;
    class EffectDamageMultiply_Passkey {
     private:
        friend EffectFactory;
        EffectDamageMultiply_Passkey() = default;
        ~EffectDamageMultiply_Passkey() = default;
    };

protected:
    Effect_Damage_Multiply() = default;
    explicit Effect_Damage_Multiply(const YAML::Node& node);

public:
    Effect_Damage_Multiply(const YAML::Node& node, EffectDamageMultiply_Passkey passkey) : Effect_Damage_Multiply(node) {}
    virtual ~Effect_Damage_Multiply() {}

    virtual const bool is_valid() const override;
    virtual float process_effect(const EffectData& data) override { return process_effect(static_cast<const EffectData_Damage_Type&>(data)); }

private:
    float process_effect(const EffectData_Damage_Type& data);
};

} // namespace Status
