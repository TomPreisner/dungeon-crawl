// An inline module registry file
#include "code/client/modules/health/damage_type_resist_health_module.h"
#include "code/client/modules/health/damage_type_resist_percent_health_module.h"
#include "code/client/modules/health/heal_type_increase_health_module.h"
#include "code/client/modules/health/status_effect_immune_health_module.h"

Module::HealthModuleFactory::HealthModuleFactory() {
    REGISTER_HEALTH_MODULE(DamageResistAmount, DamageTypeResist_HealthModule)
    REGISTER_HEALTH_MODULE(DamageResistPercent, DamageTypeResistPercent_HealthModule)
    REGISTER_HEALTH_MODULE(HealIncreaseAmount, HealTypeIncrease_HealthModule)
    REGISTER_HEALTH_MODULE(StatusEffectImmune, StatusEffectImmune_HealthModule)
}
