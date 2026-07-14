/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/

#include "damage_type_resist_percent_health_module.h"

#include "code/client/modules/descriptors/damage_type_percent_descriptor.h"
#include "code/core/log_manager.h"

namespace Module {
CREATE_LOGGER(DamageTypeResistPercent_HealthModule);

DamageTypeResistPercent_HealthModule::DamageTypeResistPercent_HealthModule(const std::string& health_module_name, const YAML::Node& data) :
    HealthModule(health_module_name, data) {
    auto damage_type_percent = std::make_shared<DamageTypePercent_Descriptor>();
    if (damage_type_percent->init_descriptor(data)) {
        assign_descriptor(damage_type_percent);
    } else {
        LOG_ERROR(DamageTypeResistPercent_HealthModule, "Failed to initialize module descriptor: \n" + YAML::Dump(data));
    }
}

void DamageTypeResistPercent_HealthModule::process_damage(code::client::messages::Damage& incoming) {
    DamageTypePercent_Descriptor* desc = get_descriptor<DamageTypePercent_Descriptor>();
    if (desc == nullptr) {
        LOG_ERROR(DamageTypeResistPercent_HealthModule, "The descriptor is null or of the wrong type");
        return; //< do nothing
    }

    if (!desc->get_damage_type().has_value() || !desc->get_amount().has_value()) {        
        LOG_WARN(DamageTypeResistPercent_HealthModule, "Invalid values in DamageTypeResistPercent_HealthModule descriptor");
        return; //< do nothing
    }

    if (incoming.damage_type() == (incoming.damage_type() & desc->get_damage_type().value())) {
        incoming.set_amount(std::max(0.f, incoming.amount() * (1.f - desc->get_amount().value())));
    } else if ((incoming.damage_type() & desc->get_damage_type().value()) != 0 && desc->get_partial_effect_amount().has_value()) {
        // if it isn't a perfect match, there might be a partial effect that is better than nothing
        incoming.set_amount(std::max(0.f, incoming.amount() * (1.f - desc->get_partial_effect_amount().value())));
    }
}
} // namespace Module
