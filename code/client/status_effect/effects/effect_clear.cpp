/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#include "code/client/status_effect/effects/effect_clear.h"

#include <algorithm>
#include <string>

#include "code/core/log_manager.h"

namespace Status {
CREATE_LOGGER(Effect_Clear);

Effect_Clear::Effect_Clear(const YAML::Node& node) {
    init(node);

    if (!is_valid()) {
        LOG_ERROR(Effect_Clear, "Node data is not valid, skipping:\n" + YAML::Dump(node));
        clear_values();
        return;
    }
}

const bool Effect_Clear::is_valid() const {
    return m_is_valid &&
            get_effect_type() == EffectType::CLEAR;
}

float Effect_Clear::process_effect(const EffectData& data) {
    if (!is_valid()) {
        LOG_ERROR(Effect_Clear, "Node data is not valid, skipping.");
        return 0.f;
    }

    const float effect_value = 0.f;
    if (m_callback) {
        m_callback(effect_value);
    }

    return effect_value;
}

} // namespace Status
