/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/

#include "effect_factory.h"

#include "effect_clear.h"
#include "effect_damage.h"
#include "effect_damage_multiply.h"
#include "effect_heal.h"
#include "effect_heal_multiply.h"
#include "effect_type.h"
#include "code/core/log_manager.h"

namespace Status {
CREATE_LOGGER(EffectFactory);
    
std::shared_ptr<Effect_Base> EffectFactory::create_effect(const YAML::Node& node) {
    assert(node.IsMap());

    YAML::Node entry = node["effect"];
    assert(entry.IsScalar());

    EffectType effect_type = EffectType::NONE;
    try {
        effect_type = convert_to_effect_type(entry.as<std::string>());
    } catch (const YAML::TypedBadConversion<std::string>& e) {
        // Not a string 
        LOG_ERROR(EffectFactory, "Status::EffectFactory - effect type for effect is not a string: " + YAML::Dump(node));
    }

    switch (effect_type) {
        case EffectType::NONE:
            LOG_ERROR(EffectFactory, "Unknown effect type: " + YAML::Dump(node));
            return nullptr;
        case EffectType::CLEAR:
        {
            Status::Effect_Clear::EffectClear_Passkey pass_key;
            return std::make_shared<Effect_Clear>(node, pass_key);
        }
        case EffectType::DAMAGE:
        {
            Status::Effect_Damage::EffectDamage_Passkey pass_key;
            return std::make_shared<Effect_Damage>(node, pass_key);
        }
        case EffectType::DAMAGE_MULTIPLY:
        {
            Status::Effect_Damage_Multiply::EffectDamageMultiply_Passkey pass_key;
            return std::make_shared<Effect_Damage_Multiply>(node, pass_key);
        }
        case EffectType::HEAL:
        {
            Status::Effect_Heal::EffectHeal_Passkey pass_key;
            return std::make_shared<Effect_Heal>(node, pass_key);
        }
        case EffectType::HEAL_MULTIPLY:
        {
            Status::Effect_Heal_Multiply::EffectHealMultiply_Passkey pass_key;
            return std::make_shared<Effect_Heal_Multiply>(node, pass_key);
        }
    }

    LOG_ERROR(EffectFactory, "Unknown effect type, How did we get here?\n " + YAML::Dump(node));
    return nullptr;
}

} // namespace Status
