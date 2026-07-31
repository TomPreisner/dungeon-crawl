/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include <functional>
#include <optional>
#include "yaml-cpp/yaml.h"

#include "base_effect.h"
#include "effect_data.h"

namespace Status {
class EffectFactory;  //< forward declare the friend class

class Effect_Heal : public Effect_Base {
private:
    friend EffectFactory;
    class EffectHeal_Passkey {
     private:
        friend EffectFactory;
        EffectHeal_Passkey() = default;
        ~EffectHeal_Passkey() = default;
    };

protected:
    Effect_Heal() = default;
    explicit Effect_Heal(const YAML::Node& node);

    const bool test_has_heal_type() const { return m_heal_type.has_value(); }
    const int32_t test_get_heal_type() const { return m_heal_type.value(); }

public:
    Effect_Heal(const YAML::Node& node, EffectHeal_Passkey passkey) : Effect_Heal(node) {}
    virtual ~Effect_Heal() {}

    virtual const bool is_valid() const override;
    virtual float process_effect(const EffectData& data) override;

protected:
    virtual void init(const YAML::Node& node);
    virtual void clear_values() override;

    std::optional<int32_t> m_heal_type;
};

} // namespace Status
