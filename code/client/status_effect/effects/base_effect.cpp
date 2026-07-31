/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#include "code/client/status_effect/effects/base_effect.h"

#include <algorithm>
#include <cassert>
#include <map>

#include "code/core/log_manager.h"

namespace Status {
CREATE_LOGGER(Effect_Base);

void Effect_Base::init(const YAML::Node& node) {
    assert(node.IsMap());

    YAML::Node entry = node["effect"];
    assert(entry.IsScalar());

    try {
        m_effect_type = convert_to_effect_type(entry.as<std::string>());
    } catch (const YAML::TypedBadConversion<std::string>& e) {
        // Not a string 
        LOG_ERROR(Effect_Base, "Status::Effect_Base - effect type for effect is not a string: " + YAML::Dump(node));
        clear_values(); 
        return;
    }

    if (m_effect_type == EffectType::NONE) {
        LOG_ERROR(Effect_Base, "Status::Effect_Base - effect type resolves to NONE, skipping: " + YAML::Dump(node));
        clear_values(); 
        return;
    }

    // amount doesn't need to be there for some effect types
    if (node["amount"]) {
        entry = node["amount"];
        if (entry.IsScalar()) {
            try {
                const float amt = entry.as<float>();
                if (amt < 0.f) {
                    LOG_ERROR(Effect_Base, "Status::Effect_Base - amount can not be negative! value: " + std::to_string(amt));
                    clear_values(); 
                    return;
                } else {
                    m_amount = amt;
                }
            } catch (const YAML::TypedBadConversion<float>& e) {
                // Not a float 
                LOG_ERROR(Effect_Base, "Status::Effect_Base - amount for effect is not a float: " + YAML::Dump(node));
                clear_values(); 
                return;
            }
        }
    }

    m_is_valid = true;
}

void Effect_Base::clear_values() {
    m_is_valid = false;
    m_effect_type = EffectType::NONE;
    m_amount = std::nullopt;
    m_callback = nullptr;
}

} // namespace Status
