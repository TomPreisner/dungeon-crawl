/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once
#include <memory>

#include "code/client/modules/descriptors/base/base_descriptor.h"

namespace Module {

class BaseModule {
public:
    BaseModule() = delete;
    BaseModule(const std::string& module_name, const YAML::Node& data);
    virtual ~BaseModule() {}

    const std::string& get_module_name() const { return m_module_name; }

protected:
    // This is a convenience function to make processing modules easier
    template<typename T>
    T* get_descriptor() { 
        return dynamic_cast<T*>(m_base_descriptor.get());
    }

    void assign_descriptor(std::shared_ptr<BaseDescriptor> desc);

private:
    std::string m_module_name;
    std::shared_ptr<BaseDescriptor> m_base_descriptor;
};

} // namespace Module
