/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include "code/client/modules/common/damage_type_module_common.h"
#include "code/client/modules/damage/damage_module.h"

namespace Module {

class DamageTypeApply_Module : public DamageTypeModule_Common, public DamageModule {
public:
    explicit DamageTypeApply_Module(const std::string& name) : DamageTypeModule_Common(name) {}

    virtual void apply_damage(Damage& outgoing) override;
};

} // namespace Module
