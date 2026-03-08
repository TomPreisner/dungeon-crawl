/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#include "code/client/status_effect/effect.h"

#include <cassert>
#include <iostream>
#include <map>

namespace Status {

EffectType convert_to_effect_type(std::string str) {
    static std::map<std::string, EffectType> s_string_to_enum = {
        {"CLEAR", EffectType::CLEAR},
        {"DAMAGE", EffectType::DAMAGE},
        {"DAMAGE_PERCENT", EffectType::DAMAGE_PERCENT},
        {"HEAL", EffectType::HEAL},
        {"HEAL_PERCENT", EffectType::HEAL_PERCENT},
        {"MULTIPLY", EffectType::MULTIPLY}
    };

    std::transform(str.begin(), str.end(), str.begin(),
                    std::toupper);

    if (s_string_to_enum.find(str) != s_string_to_enum.end()) {
        return s_string_to_enum[str];
    }
    
    std::cout << "invalid enum string: " << str << std::endl;
    return EffectType::NONE;
}

Effect::Effect(const YAML::Node& node) {
    assert(node.IsMap());

    YAML::Node entry = node["effect"];
    assert(entry.IsScalar());

    try {
        m_effect_type = convert_to_effect_type(entry.as<std::string>());    
    } catch (const YAML::TypedBadConversion<std::string>& e) {
        // Not a string 
        std::cout << "Status::Effect - effect type for effect is not a string: " << YAML::Dump(node) << std::endl;
    }

    // amount doesn't need to be there for some effect types
    if (node["amount"]) {
        entry = node["amount"];
        if (entry.IsScalar()) {
            try {
                const float amt = entry.as<float>();
                if (amt < 0.f) {
                    std::cout << "Status::Effect - amount can not be negative! value: " << amt << std::endl;
                } else {
                    m_amount = amt;
                }
            } catch (const YAML::TypedBadConversion<float>& e) {
                // Not a float 
                std::cout << "Status::Effect - amount for effect is not a float: " << YAML::Dump(node) << std::endl;
            }
        }
    }
}

float Effect::process_effect(const float amt/*= 0.f*/) {
    float effect_value = 0.f;
    switch (m_effect_type) {
        case EffectType::NONE:
            return 0.f; // early out
        case EffectType::CLEAR:
            break;
        case EffectType::DAMAGE:
        case EffectType::HEAL:
            effect_value = m_amount;
            break;
        case EffectType::DAMAGE_PERCENT:
        case EffectType::HEAL_PERCENT:
        case EffectType::MULTIPLY:
            effect_value = amt * m_amount;
            break;
    }
    
    if (m_callback) {
        m_callback(effect_value);
    }
    return effect_value;
}

} // namespace Status
