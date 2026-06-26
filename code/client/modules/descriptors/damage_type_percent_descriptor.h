/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include "code/client/modules/descriptors/damage_type_amount_descriptor.h"

namespace Module {

class DamageTypePercent_Descriptor : public DamageTypeAmount_Descriptor {
public:
    DamageTypePercent_Descriptor() : DamageTypeAmount_Descriptor() {}

    virtual bool init_module(const YAML::Node& node) override;
};

} // namespace Module
