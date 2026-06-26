/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/

#include "damage_type_resist_health_module.h"

#include "code/client/modules/descriptors/damage_type_amount_descriptor.h"
#include "code/core/log_manager.h"

namespace Module {
CREATE_LOGGER(DamageTypeResist_HealthModule);

DamageTypeResist_HealthModule::DamageTypeResist_HealthModule(const std::string& health_module_name, const YAML::Node& data) :
    HealthModule(health_module_name, data) {
    auto damage_type_amount = std::make_shared<DamageTypeAmount_Descriptor>();
    if (damage_type_amount->init_module(data)) {
        assign_descriptor(damage_type_amount);
    } else {
        LOG_ERROR(DamageTypeResist_HealthModule, "Failed to initialize module descriptor: \n" + YAML::Dump(data));
    }
}

void DamageTypeResist_HealthModule::process_damage(code::client::messages::Damage& incoming) {
    DamageTypeAmount_Descriptor* desc = get_descriptor<DamageTypeAmount_Descriptor>();
    if (desc == nullptr) {
        LOG_ERROR(DamageTypeResist_HealthModule, "The descriptor is null or of the wrong type");
        return; //< do nothing
    }

    if (!desc->get_damage_type().has_value() || !desc->get_amount().has_value()) {
        LOG_WARN(DamageTypeResist_HealthModule, "Invalid values in DamageTypeResist_HealthModule descriptor");
        return; //< do nothing
    }

    if (incoming.damage_type() == (incoming.damage_type() & desc->get_damage_type().value())) {
        incoming.set_amount(std::max(0.f, incoming.amount() - desc->get_amount().value()));
    } else if ((incoming.damage_type() & desc->get_damage_type().value()) != 0 && desc->get_partial_effect_amount().has_value()) {
        // if it isn't a perfect match, there might be a partial effect that is better than nothing
        incoming.set_amount(std::max(0.f, incoming.amount() - desc->get_partial_effect_amount().value()));
    }
}

} // namespace Module
