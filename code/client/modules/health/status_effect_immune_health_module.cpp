/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/

#include "status_effect_immune_health_module.h"
#include <random>

#include "code/client/modules/descriptors/status_effect_descriptor.h"
#include "code/client/status_effect/status_effect_library.h"
#include "code/core/log_manager.h"

namespace Module {
CREATE_LOGGER(StatusEffectImmune_HealthModule);

StatusEffectImmune_HealthModule::StatusEffectImmune_HealthModule(const std::string& health_module_name, const YAML::Node& data) :
    HealthModule(health_module_name, data) {
    auto status_effect_desc = std::make_shared<StatusEffect_Descriptor>();
    if (status_effect_desc->init_module(data)) {
        assign_descriptor(status_effect_desc);
    } else {
        LOG_ERROR(StatusEffectImmune_HealthModule, "Failed to initialize module descriptor: \n" + YAML::Dump(data));
    }
}

void StatusEffectImmune_HealthModule::process_damage(code::client::messages::Damage& incoming) {
    StatusEffect_Descriptor* desc = get_descriptor<StatusEffect_Descriptor>();
    if (desc == nullptr) {
        LOG_ERROR(StatusEffectImmune_HealthModule, "The descriptor is null or of the wrong type");
        return; //< do nothing
    }

    if (!desc->get_status_effect_type().has_value()) {        
        LOG_WARN(StatusEffectImmune_HealthModule, "Invalid status effect type in StatusEffectImmune_HealthModule descriptor");
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
                LOG_WARN(StatusEffectImmune_HealthModule, "Incoming status effect not found in library: " + status_effect_name);
                continue;
            }

            if (effect->get_type() == desc->get_status_effect_type().value()) {
                if (desc->get_percent_chance().has_value()) {
                    std::random_device device;
                    std::mt19937 generator(device()); // Mersenne Twister engine
                    std::uniform_real_distribution<> dis(0.0, 1.0);
                    if (dis(generator) > desc->get_percent_chance().value()) {
                        continue;
                    }
                }
                *(incoming.mutable_status_effect(i)) = ""; //< clear it
            }
        }
    }
}

} // namespace Module
