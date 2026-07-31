/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include <functional>
#include <optional>
#include <stdint.h>
#include "yaml-cpp/yaml.h"

#include "code/client/status_effect/effects/effect_type.h"
#include "code/client/status_effect/effects/effect_data.h"

namespace Status {

class Effect_Base {
public:
    Effect_Base() = default;
    virtual ~Effect_Base() = default;

    virtual void RegisterCallback(std::function<void(float, int32_t)> callback) { m_callback = callback; }  //< Not great, I need some checks
    virtual const EffectType get_effect_type() const { return m_effect_type; }
    virtual float process_effect(const EffectData& data) = 0;

    virtual const bool is_valid() const = 0;

protected:
    // Protected functions exposing functionality for unit tests only
    virtual const bool test_has_valid_amount() const { return m_amount.has_value(); }
    virtual const float test_get_amount() const { return m_amount.value(); }
    virtual const bool test_has_valid_callback() const { return (m_callback != nullptr); }

    virtual void init(const YAML::Node& node);
    virtual void clear_values();

    bool m_is_valid = false;
    EffectType m_effect_type = EffectType::NONE;
    std::optional<float> m_amount;
    std::function<void(float, int32_t)> m_callback;
};

} // namespace Status
