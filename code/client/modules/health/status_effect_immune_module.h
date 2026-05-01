/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include "code/client/modules/common/status_effect_module_common.h"
#include "code/client/modules/health/health_module.h"

namespace Module {

class StatusEffectImmune_Module : public StatusEffectModule_Common, public HealthModule {
public:
    explicit StatusEffectImmune_Module(const std::string& name) : StatusEffectModule_Common(name) {}

    virtual bool init_module(const YAML::Node& node) override;

    virtual void process_heal(code::client::messages::Heal& incoming) override {}
    virtual void process_damage(code::client::messages::Damage& incoming) override;
    
protected:
    virtual void clear_values() override;

    std::optional<float> m_percent_chance_amount;
};

} // namespace Module
