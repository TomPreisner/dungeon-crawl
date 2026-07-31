/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#include "effect_type.h"

namespace Status {

// the string is not a const and not a reference because it will be modified
EffectType convert_to_effect_type(std::string str) {
    static std::map<std::string, EffectType> s_string_to_enum = {
        {"CLEAR", EffectType::CLEAR},
        {"DAMAGE", EffectType::DAMAGE},
        {"DAMAGE_MULTIPLY", EffectType::DAMAGE_MULTIPLY},
        {"HEAL", EffectType::HEAL},
        {"HEAL_MULTIPLY", EffectType::HEAL_MULTIPLY}
    };

    std::transform(str.begin(), str.end(), str.begin(),
                    [](char c) { return std::toupper(c); });

    if (s_string_to_enum.find(str) != s_string_to_enum.end()) {
        return s_string_to_enum[str];
    }

    return EffectType::NONE;
}

std::string to_string(const EffectType type) {
    static std::map<EffectType, std::string> s_enum_to_string = {
        {EffectType::CLEAR, "CLEAR"},
        {EffectType::DAMAGE, "DAMAGE"},
        {EffectType::DAMAGE_MULTIPLY, "DAMAGE_MULTIPLY"},
        {EffectType::HEAL, "HEAL"},
        {EffectType::HEAL_MULTIPLY, "HEAL_MULTIPLY"}
    };
    
    if (s_enum_to_string.find(type) != s_enum_to_string.end()) {
        return s_enum_to_string[type];
    }

    return "UNKNOWN";
}

} // namespace Status
