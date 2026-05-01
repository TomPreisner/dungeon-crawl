/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include "code/client/modules/common/status_effect_module_common.h"
#include "code/client/modules/damage/damage_module.h"

namespace Module {

class StatusEffectApply_Module : public StatusEffectModule_Common, public DamageModule {
public:
    explicit StatusEffectApply_Module(const std::string& name) : StatusEffectModule_Common(name) {}

    virtual bool init_module(const YAML::Node& node) override;
    virtual void apply_damage(code::client::messages::Damage& outgoing) override;
};

} // namespace Module
