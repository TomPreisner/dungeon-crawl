/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#include "code/client/status_effect/effects/effect_heal_multiply.h"

#include <algorithm>
#include <cassert>
#include <map>

#include "code/client/messages/proto/heal.pb.h"
#include "code/core/log_manager.h"

namespace Status {
CREATE_LOGGER(Effect_Heal_Multiply);

Effect_Heal_Multiply::Effect_Heal_Multiply(const YAML::Node& node) {
    init(node);

    if (!is_valid()) {
        LOG_ERROR(Effect_Heal_Multiply, "Node data is not valid, skipping:\n" + YAML::Dump(node));
        clear_values();
        return;
    }
}

const bool Effect_Heal_Multiply::is_valid() const {
    return m_is_valid &&
            get_effect_type() == EffectType::HEAL_MULTIPLY;
}

float Effect_Heal_Multiply::process_effect(const EffectData_Heal_Type& data) {
    if (!is_valid()) {
        LOG_ERROR(Effect_Heal_Multiply, "Node data is not valid, skipping.");
        return 0.f;
    }

    if (!m_amount.has_value()) {
        LOG_ERROR(Effect_Heal_Multiply, "Status::Effect_Heal_Multiply - m_amount is not valid.");
        return 0.f;
    }

    if (!m_heal_type.has_value()) {
        LOG_ERROR(Effect_Heal_Multiply, "Status::Effect_Heal_Multiply - m_heal_type is not valid.");
        return 0.f;
    }

    float effect_value = 0.f;

    if (data.heal_type == (data.heal_type & m_heal_type.value())) {
        effect_value = data.amount * m_amount.value();

        if (m_callback) {
            m_callback(effect_value);
        }
    }
    return effect_value;
}

} // namespace Status
