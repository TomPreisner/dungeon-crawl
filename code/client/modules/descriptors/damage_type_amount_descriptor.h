/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include "code/client/modules/descriptors/base/damage_type_descriptor_base.h"

namespace Module {

class DamageTypeAmount_Descriptor : public DamageTypeDescriptor_Base {
public:
    DamageTypeAmount_Descriptor() : DamageTypeDescriptor_Base() {}

    virtual bool init_module(const YAML::Node& node) override;
    
    std::optional<float> get_partial_effect_amount() const { return m_partial_effect_amount; }
protected:
    virtual void clear_values() override;

    std::optional<float> m_partial_effect_amount;
};

} // namespace Module
