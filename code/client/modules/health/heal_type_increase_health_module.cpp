/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/

#include "heal_type_increase_health_module.h"

#include "code/client/modules/descriptors/heal_type_amount_descriptor.h"
#include "code/core/log_manager.h"

namespace Module {
CREATE_LOGGER(HealTypeIncrease_HealthModule);

HealTypeIncrease_HealthModule::HealTypeIncrease_HealthModule(const std::string& health_module_name, const YAML::Node& data) :
    HealthModule(health_module_name, data) {
    auto heal_type_amount = std::make_shared<HealTypeAmount_Descriptor>();
    if (heal_type_amount->init_descriptor(data)) {
        assign_descriptor(heal_type_amount);
    } else {
        LOG_ERROR(HealTypeIncrease_HealthModule, "Failed to initialize module descriptor: \n" + YAML::Dump(data));
    }
}

void HealTypeIncrease_HealthModule::process_heal(code::client::messages::Heal& incoming) {
    HealTypeAmount_Descriptor* desc = get_descriptor<HealTypeAmount_Descriptor>();
    if (desc == nullptr) {
        LOG_ERROR(HealTypeIncrease_HealthModule, "The descriptor is null or of the wrong type");
        return; //< do nothing
    }

    if (!desc->get_heal_type().has_value() || !desc->get_amount().has_value()) {
        LOG_WARN(HealTypeIncrease_HealthModule, "Invalid values in HealTypeIncrease_HealthModule descriptor");
        return; //< do nothing
    }

    if (incoming.type() == desc->get_heal_type().value()) {
        incoming.set_amount(incoming.amount() + desc->get_amount().value());
    }
}
} // namespace Module
