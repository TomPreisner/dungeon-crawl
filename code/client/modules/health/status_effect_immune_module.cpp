/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/

#include "status_effect_immune_module.h"
#include <random>

#include "code/client/status_effect/status_effect_library.h"
#include "code/core/log_manager.h"

namespace Module {
CREATE_LOGGER(StatusEffectImmune_Module);

bool StatusEffectImmune_Module::init_module(const YAML::Node& node) {
    if (StatusEffectModule_Common::init_module(node)) {
        if (!m_status_effect_type.has_value()) {
            LOG_ERROR(StatusEffectImmune_Module, "Invalid status effect type in " + get_module_name() + ". This module will not function.")
            clear_values();
            return false;
        }
        
        // An missing percent cchance means it is 100%
        const YAML::Node& percentchance_node = node["percent_chance"];
        if (percentchance_node) {
            try {
                m_percent_chance_amount = percentchance_node.as<float>();
            } catch (const YAML::TypedBadConversion<float>& e) {
                LOG_ERROR(StatusEffectImmune_Module, "Value in \"percent_chance\" is not a float, skipping: " + YAML::Dump(percentchance_node));
                clear_values();
                return false;
            }
            if (m_percent_chance_amount.value() <= 0.f || m_percent_chance_amount.value() > 1.f) {
                LOG_ERROR(StatusEffectImmune_Module, "Value in \"percent_chance\" is invalid, value: " + std::to_string(m_percent_chance_amount.value()));
                clear_values();
                return false;

            }
        }
        return true;
    }
    return false;
}

void StatusEffectImmune_Module::process_damage(Damage& incoming) {
    if (!m_status_effect_type.has_value()) {        
        LOG_WARN(StatusEffectImmune_Module, "Invalid status effect type in " + get_module_name())
        return; //< do nothing
    }

    if (incoming.status_effect_size() > 0) {
        const int size = incoming.status_effect_size();
        for (int i = 0; i < size; ++i) {
            const std::string& status_effect_name = incoming.status_effect(i);
            if (status_effect_name.empty()) {
                continue; //< it might have been removed
            }

            auto effect = Status::StatusEffectLibrary::get_Instance()->get_status_effect(status_effect_name);
            if (!effect.has_value()) {
                LOG_WARN(StatusEffectImmune_Module, "Incoming status effect not found in library: " + status_effect_name);
                continue;
            }

            if (effect->get_type() == m_status_effect_type.value()) {
                if (m_percent_chance_amount.has_value()) {
                    std::random_device device;
                    std::mt19937 generator(device()); // Mersenne Twister engine
                    std::uniform_real_distribution<> dis(0.0, 1.0);
                    if (dis(generator) > m_percent_chance_amount.value()) {
                        continue;
                    }
                }
                *(incoming.mutable_status_effect(i)) = ""; //< clear it
            }
        }
    }
}

void StatusEffectImmune_Module::clear_values() {
    StatusEffectModule_Common::clear_values();
    m_percent_chance_amount = std::nullopt;
}
} // namespace Module
