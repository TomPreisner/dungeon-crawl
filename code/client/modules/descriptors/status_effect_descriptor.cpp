/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/

#include "status_effect_descriptor.h"
#include <random>

#include "code/client/status_effect/status_effect_library.h"
#include "code/core/log_manager.h"

namespace Module {
CREATE_LOGGER(StatusEffect_Descriptor);

bool StatusEffect_Descriptor::init_descriptor(const YAML::Node& node) {
    if (!StatusEffectDescriptor_Base::init_descriptor(node)) {
         return false;  
    }

    // A missing percent chance means it is 100%
    const YAML::Node& percentchance_node = node["percent_chance"];
    if (percentchance_node) {
        try {
            m_percent_chance = percentchance_node.as<float>();
        } catch (const YAML::TypedBadConversion<float>& e) {
            LOG_ERROR(StatusEffect_Descriptor, "Value in \"percent_chance\" is not a float, skipping: " + YAML::Dump(percentchance_node));
            clear_values();
            return false;
        }
        if (m_percent_chance.value() <= 0.f || m_percent_chance.value() > 1.f) {
            LOG_ERROR(StatusEffect_Descriptor, "Value in \"percent_chance\" is invalid, value: " + std::to_string(m_percent_chance.value()));
            clear_values();
            return false;

        }
    }
    return true;
}

void StatusEffect_Descriptor::clear_values() {
    StatusEffectDescriptor_Base::clear_values();
    m_percent_chance = std::nullopt;
}
} // namespace Module
