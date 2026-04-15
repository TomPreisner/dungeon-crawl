/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include "code/client/modules/common/damage_type_module_common.h"
#include "code/client/modules/health/health_module.h"

namespace Module {

class DamageTypeResist_Module : public DamageTypeModule_Common, public HealthModule {
public:
    explicit DamageTypeResist_Module(const std::string& name) : DamageTypeModule_Common(name) {}

    virtual bool init_module(const YAML::Node& node) override;

    virtual void process_heal(Heal& incoming) override {}
    virtual void process_damage(Damage& incoming) override;
    
protected:
    virtual void clear_values() override;

    std::optional<float> m_partial_effect_amount;
};

} // namespace Module
