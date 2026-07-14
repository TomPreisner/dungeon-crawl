/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include "code/client/modules/descriptors/base/status_effect_descriptor_base.h"

namespace Module {

class StatusEffect_Descriptor : public StatusEffectDescriptor_Base {
public:
    StatusEffect_Descriptor() : StatusEffectDescriptor_Base() {}

    virtual bool init_descriptor(const YAML::Node& node) override;
    std::optional<float> get_percent_chance() const { return m_percent_chance; }

protected:
    virtual void clear_values() override;

    std::optional<float> m_percent_chance;
};

} // namespace Module
