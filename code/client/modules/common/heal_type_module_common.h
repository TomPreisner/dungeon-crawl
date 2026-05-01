/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include <optional>
#include "code/client/modules/common/base_module.h"
#include "code/client/messages/proto/heal.pb.h"

namespace Module {
    
class HealTypeModule_Common : public BaseModule {
public:
    explicit HealTypeModule_Common(const std::string& name) : BaseModule(name) {}

    virtual bool init_module(const YAML::Node& node) override;

protected:
    virtual void clear_values() override;

    std::optional<code::client::messages::Heal::HealType> m_heal_type;
    std::optional<float> m_amount;
};

} // namespace Module
