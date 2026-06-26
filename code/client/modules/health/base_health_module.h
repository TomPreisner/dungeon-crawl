/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once
#include <memory>

#include "code/client/messages/proto/damage.pb.h"
#include "code/client/messages/proto/heal.pb.h"
#include "code/client/modules/base/base_module.h"

namespace Module {

class HealthModule : public BaseModule {
public:
    HealthModule() = delete;
    HealthModule(const std::string& module_name, const YAML::Node& data) : 
        BaseModule(module_name, data) {}
    virtual ~HealthModule() {}

    virtual void process_heal(code::client::messages::Heal& incoming) {}
    virtual void process_damage(code::client::messages::Damage& incoming) {}
};

} // namespace Module
