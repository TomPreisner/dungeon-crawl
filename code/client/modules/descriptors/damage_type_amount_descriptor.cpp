/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/

#include "damage_type_amount_descriptor.h"

#include "code/core/log_manager.h"

namespace Module {
CREATE_LOGGER(DamageTypeAmount_Descriptor);

bool DamageTypeAmount_Descriptor::init_descriptor(const YAML::Node& node) {
    if (!DamageTypeDescriptor_Base::init_descriptor(node)) {
        return false;
    }

    if (!m_amount.has_value() || m_amount.value() < 0.f) {
        LOG_ERROR(DamageTypeAmount_Descriptor, "amount is invalid: " + std::to_string(m_amount.value_or(-0.001f)));
        clear_values();
        return false;
    }

    const YAML::Node& partialeffect_node = node["partial_effect_amount"];
    if (partialeffect_node) {
        try {
            m_partial_effect_amount = partialeffect_node.as<float>();
        } catch (const YAML::TypedBadConversion<float>& e) {
            LOG_ERROR(DamageTypeAmount_Descriptor, "Value in \"partial_effect_amount\" is not a float, skipping: " + YAML::Dump(partialeffect_node));
            clear_values();
            return false;
        }
        
        if (m_partial_effect_amount.value() < 0.f || m_partial_effect_amount.value() > m_amount.value()) {
            LOG_ERROR(DamageTypeAmount_Descriptor, "Value in \"partial_effect_amount\" is invalid, skipping: " + std::to_string(m_partial_effect_amount.value()));
            clear_values();
            return false;
        }
    }

    return true;
}

void DamageTypeAmount_Descriptor::clear_values() {
    DamageTypeDescriptor_Base::clear_values();
    m_partial_effect_amount = std::nullopt;
}

} // namespace Module
