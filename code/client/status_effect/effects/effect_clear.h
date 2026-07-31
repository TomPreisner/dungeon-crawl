/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include <functional>
#include "yaml-cpp/yaml.h"
#include "base_effect.h"
#include "effect_data.h"

namespace Status {
class EffectFactory;  //< forward declare the friend class

class Effect_Clear : public Effect_Base {
private:
    friend EffectFactory;
    class EffectClear_Passkey {
     private:
        friend EffectFactory;
        EffectClear_Passkey() = default;
        ~EffectClear_Passkey() = default;
    };

protected:
    explicit Effect_Clear(const YAML::Node& node);

public:
    Effect_Clear(const YAML::Node& node, EffectClear_Passkey passkey) : Effect_Clear(node) {}
    virtual ~Effect_Clear() {}

    virtual const bool is_valid() const override;
    virtual float process_effect(const EffectData& data) override;
};

} // namespace Status
