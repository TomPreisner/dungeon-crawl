/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/

#include "damage_type_apply_damage_module.h"

#include "code/client/modules/descriptors/damage_type_amount_descriptor.h"
#include "code/core/log_manager.h"

namespace Module {
CREATE_LOGGER(DamageTypeApply_DamageModule);

DamageTypeApply_DamageModule::DamageTypeApply_DamageModule(const std::string& damage_module_name, const YAML::Node& data) :
    DamageModule(damage_module_name, data) {
    auto damage_type_amount = std::make_shared<DamageTypeAmount_Descriptor>();
    if (damage_type_amount->init_descriptor(data)) {
        assign_descriptor(damage_type_amount);
    } else {
        LOG_ERROR(DamageTypeApply_DamageModule, "Failed to initialize module descriptor: \n" + YAML::Dump(data));
    }
}

void DamageTypeApply_DamageModule::apply_damage(code::client::messages::Damage& outgoing) {
    DamageTypeAmount_Descriptor* desc = get_descriptor<DamageTypeAmount_Descriptor>();
    if (desc == nullptr) {
        LOG_ERROR(DamageTypeApply_DamageModule, "The descriptor is null or of the wrong type");
        return; //< do nothing
    }

    if (!desc->get_damage_type().has_value() || !desc->get_amount().has_value()) {        
        LOG_WARN(DamageTypeApply_DamageModule, "Invalid values in " + get_module_name())
        return; //< do nothing
    }

    outgoing.set_damage_type(outgoing.damage_type() | desc->get_damage_type().value());
    outgoing.set_amount(outgoing.amount() + desc->get_amount().value());
}
} // namespace Module
