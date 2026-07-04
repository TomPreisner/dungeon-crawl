/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include <optional>
#include "code/client/modules/descriptors/base/base_descriptor.h"
#include "code/client/messages/proto/heal.pb.h"

namespace Module {
    
class HealTypeDescriptor_Base : public BaseDescriptor {
public:
    HealTypeDescriptor_Base() : BaseDescriptor() {}

    virtual bool init_descriptor(const YAML::Node& node) override;

    std::optional<code::client::messages::Heal::HealType> get_heal_type() const { return m_heal_type; }
    std::optional<float> get_amount() const { return m_amount; }
protected:
    virtual void clear_values() override;

    std::optional<code::client::messages::Heal::HealType> m_heal_type;
    std::optional<float> m_amount;
};

} // namespace Module
