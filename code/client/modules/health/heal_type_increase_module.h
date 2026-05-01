/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include "code/client/modules/common/heal_type_module_common.h"
#include "code/client/modules/health/health_module.h"

namespace Module {

class HealTypeIncrease_Module : public HealTypeModule_Common, public HealthModule {
public:
    explicit HealTypeIncrease_Module(const std::string& name) : HealTypeModule_Common(name) {}

    virtual bool init_module(const YAML::Node& node) override;

    virtual void process_heal(code::client::messages::Heal& incoming) override;
    virtual void process_damage(code::client::messages::Damage& incoming) override {}
};

} // namespace Module
