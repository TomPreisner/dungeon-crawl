/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include <functional>
#include <optional>
#include "yaml-cpp/yaml.h"

#include "code/client/status_effect/effects/base_effect.h"
#include "code/client/status_effect/effects/effect_data.h"

namespace Status {
class EffectFactory;  //< forward declare the friend class

class Effect_Damage : public Effect_Base {
private:
    friend EffectFactory;
    class EffectDamage_Passkey {
     private:
        friend EffectFactory;
        EffectDamage_Passkey() = default;
        ~EffectDamage_Passkey() = default;
    };

protected:
    Effect_Damage() = default;
    explicit Effect_Damage(const YAML::Node& node);

    const bool test_has_damage_type() const { return m_damage_type.has_value(); }
    const int32_t test_get_damage_type() const { return m_damage_type.value(); }

public:
    Effect_Damage(const YAML::Node& node, EffectDamage_Passkey passkey) : Effect_Damage(node) {}
    virtual ~Effect_Damage() {}

    virtual const bool is_valid() const override;
    virtual float process_effect(const EffectData& data) override;

protected:
    virtual void init(const YAML::Node& node);
    virtual void clear_values() override;

    std::optional<int32_t> m_damage_type;   //< this is a bit field
};

} // namespace Status
