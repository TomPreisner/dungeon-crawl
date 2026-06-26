/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include <optional>
#include "code/client/modules/descriptors/base/base_descriptor.h"
#include "code/client/status_effect/status_effect.h"

namespace Module {

class StatusEffectDescriptor_Base : public BaseDescriptor {
public:
    StatusEffectDescriptor_Base() : BaseDescriptor() {}

    virtual bool init_module(const YAML::Node& node) override;

    std::optional<std::string> get_status_effect_name() const { return m_status_effect_name; }
    std::optional<Status::StatusEffectType> get_status_effect_type() const { return m_status_effect_type; }
protected:
    virtual void clear_values() override;

    std::optional<std::string> m_status_effect_name;
    std::optional<Status::StatusEffectType> m_status_effect_type;
};

} // namespace Module

