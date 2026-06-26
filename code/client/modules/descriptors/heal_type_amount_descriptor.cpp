/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/

#include "heal_type_amount_descriptor.h"

#include "code/core/log_manager.h"

namespace Module {
CREATE_LOGGER(HealTypeAmount_Descriptor);

bool HealTypeAmount_Descriptor::init_module(const YAML::Node& node) {
    if (!HealTypeDescriptor_Base::init_module(node)) {
        return false;
    }

    if (!m_amount.has_value() || m_amount.value() < 0.f) {
        LOG_ERROR(HealTypeAmount_Descriptor, "amount is invalid: " + std::to_string(m_amount.value_or(-0.001f)));
        clear_values();
        return false;
    }

    return true;
}

} // namespace Module
