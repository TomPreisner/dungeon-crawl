/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include <optional>
#include "code/client/modules/descriptors/base/base_descriptor.h"

namespace Module {
    
class DamageTypeDescriptor_Base : public BaseDescriptor {
public:
    DamageTypeDescriptor_Base() : BaseDescriptor() {}

    virtual bool init_module(const YAML::Node& node) override;

    std::optional<int32_t> get_damage_type() const { return m_damage_type; }
    std::optional<float> get_amount() const { return m_amount; }
protected:
    virtual void clear_values() override;

    std::optional<int32_t> m_damage_type;   //< this is a bit field
    std::optional<float> m_amount;
};

} // namespace Module
