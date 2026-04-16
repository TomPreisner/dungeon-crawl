/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include <list>
#include <memory>
#include "code/client/damage/proto/damage.pb.h"
#include "code/client/health/proto/heal.pb.h"
#include "code/client/modules/health/health_module.h"

namespace Status {
    class StatusEffectManager;
}

// The health manager is a class that manages incoming damage to a character
//  processing the damage and applying status effects. It consists of series of 
//  priority based modules that process the damage. These filter the damage and 
//  strips inactive items before finally applying it to the character.
// For example, the character has poison immunity and armor, and a melee attack that
//  has a status effect is applied. The armor takes a portion of the melee and the 
//  poison resistence strips the poison status effect from the incoming damage.
class HealthManager {
public:
    HealthManager();
    virtual ~HealthManager() {}

    bool init_manager(std::shared_ptr<Status::StatusEffectManager> status_manager, const YAML::Node& data);

    bool add_module(const std::string& uuid_owner, const std::string& module_name, const YAML::Node& data);

    bool remove_module(const std::string& uuid_owner, const std::string& module_name);
    bool remove_modules_from_owner(const std::string& m_uuid_owner);

    void apply_heal(std::list<Heal>& incoming_list);
    void apply_heal(const Heal& incoming);
    
    void apply_damage(std::list<Damage>& incoming_list);
    void apply_damage(const Damage& incoming);

private:
    typedef std::map<std::string, std::list<std::sharedptr<Module::HealthModule>>> HealthModuleMap;
    HealthModuleMap m_health_module;
    bool m_initialized = false;

    float m_total_health = 0.f;
    float m_health = 0.f;

    std::string m_uuid_string;
};
