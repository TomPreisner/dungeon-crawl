/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include <memory>

#include "code/client/status_effect/effects/base_effect.h"

namespace Status {

class EffectFactory {
private:
    // The passkey pattern is necessary here to use the the unique_ptr without
    //  exposing a constructor that other classes can use to create an instance
    class EffectFactoryPasskey {
     private:
        friend EffectFactory;
        EffectFactoryPasskey() = default;
        ~EffectFactoryPasskey() = default;
    };
public:
    EffectFactory(EffectFactoryPasskey passkey) : EffectFactory() { }
    ~EffectFactory() = default;

    static EffectFactory* get_Instance() {
        static EffectFactory::EffectFactoryPasskey s_passkey;
        static std::unique_ptr<EffectFactory> s_instance = std::make_unique<EffectFactory>(s_passkey);
        return s_instance.get();
    }

    std::shared_ptr<Effect_Base> create_effect(const YAML::Node& node);
private:
    EffectFactory() = default;
};

} // namespace Status
