/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include <functional>
#include "yaml-cpp/yaml.h"

namespace Status {

enum class EffectType {
    NONE,
    CLEAR,
    DAMAGE,
    DAMAGE_PERCENT,
    HEAL,
    HEAL_PERCENT,
    MULTIPLY
};

class Effect {
public:
    explicit Effect(const YAML::Node& node);
    virtual ~Effect() {}

    virtual void RegisterCallback(std::function<void(float)> callback) { m_callback = callback; }  //< Not great, I need some checks
    virtual const EffectType get_effect_type() const { return m_effect_type; }
    virtual float process_effect(const float amt = 0.f);

protected:
    const bool test_has_valid_amount() const { return m_amount >= 0.f; }
    const float test_get_amount() const { return m_amount; }
    const bool test_has_valid_callback() const { return (m_callback != nullptr); }

private:
    EffectType m_effect_type;
    float m_amount = -1.0;
    std::function<void(float)> m_callback;
};

} // namespace Status
