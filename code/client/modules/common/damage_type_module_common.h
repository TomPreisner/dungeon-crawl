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
    std::optional<int32_t> m_damage_type;   //< this is a bit field
    std::optional<float> m_amount;
};

} // namespace Module


/*
Turn this into a DamageTypeModule_Common.  From there there will be a resist amount, resist percent, and an apply.
The unit tests for the module factory should have a yaml file to instantiate all of the registered modules. if any 
modules are registered and NOT in the yaml the test should fail.
In addition, the constructor should change to a default constructor with an `init` function. that is inherited from 
Base Module. that virtual function should be declared final in the DamageTypeModule Common  and the function should 
return a bool success code.  the unit test should check the results of that function to validate that the 
initialization from the yaml file is still valid.

There should be a second set of data coming through the yaml unit test file that is a mix of bad data (good data being
validated in the previous test) to validate that bad data is flagged properly. This can be put into a map of data sequences
to make sure that there is bad data for all of the modules registered. */