/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include "code/client/modules/health/damage_type_resist_module.h"

namespace Module {

class DamageTypeResistPercent_Module : public DamageTypeResist_Module {
public:
    explicit DamageTypeResistPercent_Module(const std::string& name) : DamageTypeResist_Module(name) {}

    virtual bool init_module(const YAML::Node& node) override;

    virtual void process_damage(Damage& incoming) override;
};

} // namespace Module
