/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include "code/client/messages/proto/damage.pb.h"
#include "code/client/modules/base/base_module.h"

namespace Module {

class DamageModule : public BaseModule {
public:
    DamageModule() = default;
    DamageModule(const std::string& module_name, const YAML::Node& data) : 
        BaseModule(module_name, data) {}
    virtual ~DamageModule() {}

    virtual void apply_damage(code::client::messages::Damage& outgoing) {}
};

} // namespace Module
