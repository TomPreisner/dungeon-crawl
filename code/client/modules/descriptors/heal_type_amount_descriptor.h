/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include "code/client/modules/descriptors/base/heal_type_descriptor_base.h"

namespace Module {

class HealTypeAmount_Descriptor : public HealTypeDescriptor_Base {
public:
    HealTypeAmount_Descriptor() : HealTypeDescriptor_Base() {}

    virtual bool init_descriptor(const YAML::Node& node) override;
};

} // namespace Module
