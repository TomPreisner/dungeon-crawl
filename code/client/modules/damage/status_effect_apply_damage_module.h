/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include "code/client/modules/damage/base_damage_module.h"

namespace Module {

class StatusEffectApply_DamageModule : public DamageModule {
public:
    StatusEffectApply_DamageModule() = delete;
    StatusEffectApply_DamageModule(const std::string& damage_module_name, const YAML::Node& data);

    virtual void apply_damage(code::client::messages::Damage& outgoing) override;
};

} // namespace Module
