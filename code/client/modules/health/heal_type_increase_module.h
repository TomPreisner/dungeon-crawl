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

    virtual void process_heal(Heal& incoming) override;
    virtual void process_damage(Damage& incoming) override {}
};

} // namespace Module
