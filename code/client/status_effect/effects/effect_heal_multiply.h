/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include <functional>
#include <optional>
#include "yaml-cpp/yaml.h"

#include "effect_heal.h"
#include "effect_data.h"

namespace Status {
class EffectFactory;  //< forward declare the friend class

class Effect_Heal_Multiply : public Effect_Heal {
private:
    friend EffectFactory;
    class EffectHealMultiply_Passkey {
     private:
        friend EffectFactory;
        EffectHealMultiply_Passkey() = default;
        ~EffectHealMultiply_Passkey() = default;
    };

protected:
    Effect_Heal_Multiply() = default;
    explicit Effect_Heal_Multiply(const YAML::Node& node);

public:
    Effect_Heal_Multiply(const YAML::Node& node, EffectHealMultiply_Passkey passkey) : Effect_Heal_Multiply(node) {}
    virtual ~Effect_Heal_Multiply() {}

    virtual const bool is_valid() const override;
    virtual float process_effect(const EffectData& data) override { return process_effect(static_cast<const EffectData_Heal_Type&>(data)); }

private:
    float process_effect(const EffectData_Heal_Type& data);
};

} // namespace Status
