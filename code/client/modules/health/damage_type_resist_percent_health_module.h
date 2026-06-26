/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include "code/client/modules/health/base_health_module.h"

namespace Module {

class DamageTypeResistPercent_HealthModule : public HealthModule {
public:
    DamageTypeResistPercent_HealthModule() = delete;
    DamageTypeResistPercent_HealthModule(const std::string& health_module_name, const YAML::Node& data);

    virtual void process_heal(code::client::messages::Heal& incoming) override {}
    virtual void process_damage(code::client::messages::Damage& incoming) override;
};

} // namespace Module
