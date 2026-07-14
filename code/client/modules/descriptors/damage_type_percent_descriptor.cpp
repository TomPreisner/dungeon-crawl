/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/

#include "damage_type_percent_descriptor.h"

#include "code/core/log_manager.h"

namespace Module {
CREATE_LOGGER(DamageTypePercent_Descriptor);

bool DamageTypePercent_Descriptor::init_descriptor(const YAML::Node& node) {
    if (!DamageTypeAmount_Descriptor::init_descriptor(node)) {
        return false;
    }
    
    if (m_amount.value() < 0.f || m_amount.value() > 1.f) {
        LOG_ERROR(DamageTypePercent_Descriptor, "Invalid damage amount value: " + std::to_string(m_amount.value()))
        clear_values();
        return false;
    }
    
    if (m_partial_effect_amount.has_value() && 
        (m_partial_effect_amount.value() < 0.f || m_partial_effect_amount.value() > 1.f)) {
        LOG_ERROR(DamageTypePercent_Descriptor, "Invalid damage partial effect amount value: " + std::to_string(m_partial_effect_amount.value()))
        clear_values();
        return false;
    }

    return true;
}

} // namespace Module
