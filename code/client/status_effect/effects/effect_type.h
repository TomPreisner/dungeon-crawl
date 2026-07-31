/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include <functional>
#include "yaml-cpp/yaml.h"

namespace Status {

enum class EffectType {
    NONE,
    CLEAR,
    DAMAGE,
    DAMAGE_MULTIPLY,
    HEAL,
    HEAL_MULTIPLY,
};

// the string is not a const and not a reference because it will be modified
EffectType convert_to_effect_type(std::string str);
std::string to_string(const EffectType type);

} // namespace Status
