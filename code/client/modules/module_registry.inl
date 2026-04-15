// An inline module registry file
#include "code/client/modules/damage/damage_type_apply_module.h"
#include "code/client/modules/damage/status_effect_apply_module.h"
#include "code/client/modules/health/damage_type_resist_module.h"
#include "code/client/modules/health/damage_type_resist_percent_module.h"
#include "code/client/modules/health/heal_type_increase_module.h"
#include "code/client/modules/health/status_effect_immune_module.h"

Module::ModuleFactory::ModuleFactory() {
    REGISTER_MODULE(DamageApply, DamageTypeApply_Module)
    REGISTER_MODULE(DamageResistAmount, DamageTypeResist_Module)
    REGISTER_MODULE(DamageResistPercent, DamageTypeResistPercent_Module)
    REGISTER_MODULE(HealIncreaseAmount, HealTypeIncrease_Module)
    REGISTER_MODULE(StatusEffectApply, StatusEffectApply_Module)
    REGISTER_MODULE(StatusEffectImmune, StatusEffectImmune_Module)
}
