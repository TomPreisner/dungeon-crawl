/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include <optional>
#include "code/client/modules/common/base_module.h"

namespace Module {
    
class DamageTypeModule_Common : public BaseModule {
public:
    explicit DamageTypeModule_Common(const std::string& name) : BaseModule(name) {}

    virtual bool init_module(const YAML::Node& node) override;

protected:
    virtual void clear_values() override;

    std::optional<int32_t> m_damage_type;   //< this is a bit field
    std::optional<float> m_amount;
};

} // namespace Module
