/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/

#include "status_effect_apply_damage_module.h"

#include "code/client/modules/descriptors/status_effect_descriptor.h"
#include "code/core/log_manager.h"

namespace Module {
CREATE_LOGGER(StatusEffectApply_DamageModule);

StatusEffectApply_DamageModule::StatusEffectApply_DamageModule(const std::string& damage_module_name, const YAML::Node& data) :
    DamageModule(damage_module_name, data) {
    auto damage_type_amount = std::make_shared<StatusEffect_Descriptor>();
    if (damage_type_amount->init_module(data)) {
        assign_descriptor(damage_type_amount);
    } else {
        LOG_ERROR(StatusEffectApply_DamageModule, "Failed to initialize module descriptor: \n" + YAML::Dump(data));
    }
}

void StatusEffectApply_DamageModule::apply_damage(code::client::messages::Damage& outgoing) {
    StatusEffect_Descriptor* desc = get_descriptor<StatusEffect_Descriptor>();
    if (desc == nullptr) {
        LOG_ERROR(StatusEffectApply_DamageModule, "The descriptor is null or of the wrong type");
        return; //< do nothing
    }

    if (!desc->get_status_effect_name().has_value()) {
        LOG_WARN(StatusEffectApply_DamageModule, "Invalid status effect name in " + get_module_name())
        return; //< do nothing
    }

    std::string* value = outgoing.add_status_effect();
    if (value == nullptr) {
        LOG_WARN(StatusEffectApply_DamageModule, "Unable to add status \"" + desc->get_status_effect_name().value() + "\" in " + get_module_name())
        return; //< do nothing
    }
    *value = desc->get_status_effect_name().value();
}
} // namespace Module
