/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include <optional>
#include "code/client/modules/common/base_module.h"
#include "code/client/status_effect/status_effect.h"

namespace Module {
    
class StatusEffectModule_Common : public BaseModule {
public:
    explicit StatusEffectModule_Common(const std::string& name) : BaseModule(name) {}

    virtual bool init_module(const YAML::Node& node) override;

protected:
    std::optional<std::string> m_status_effect_name;
    std::optional<Status::StatusEffectType> m_status_effect_type;
};

} // namespace Module

