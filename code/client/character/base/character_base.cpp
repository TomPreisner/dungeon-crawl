/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/

#include "character_base.h"


//This should take a link to a yaml node that defines the character and find the health manager node in there 
CharacterBase::CharacterBase(CharacterType type) : m_type(type) {
//    HealthManager  init_manager(core::MessageSwitchboard& switchboard, const YAML::Node& data);
//    Status::StatusEffectManager  init_manager(const core::MessageSwitchboard& switchboard);
    m_health_manager = std::make_shared<HealthManager>();

    m_status_manager = std::make_shared<Status::StatusEffectManager>();
    m_status_manager->init_manager(m_switchboard);
}

void CharacterBase::on_update(const std::chrono::milliseconds& dt) {
    m_behavior_tree.tickOnce();
}
