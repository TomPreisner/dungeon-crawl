/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include <chrono>
#include <list>
#include <memory>
#include <mutex>
#include <queue>
#include "code/client/messages/health_messages.h"
#include "code/client/messages/status_messages.h"
#include "code/client/messages/proto/damage.pb.h"
#include "code/client/messages/proto/heal.pb.h"
#include "code/client/modules/health/base_health_module.h"
#include "code/core/message_system/message_publisher.h"
#include "code/core/message_system/message_subscriber.h"
#include "code/core/message_system/message_switchboard.h"
#include "yaml-cpp/yaml.h"

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

    bool init_manager(core::MessageSwitchboard& switchboard, const YAML::Node& data);
    void update_manager(const std::chrono::milliseconds& dt);

    bool add_module(const std::string& uuid_owner, const std::string& module_name, const YAML::Node& data);

    bool remove_module(const std::string& uuid_owner, const std::string& module_name);
    bool remove_modules_from_owner(const std::string& m_uuid_owner);

    void apply_heal(std::list<code::client::messages::Heal>& incoming_list);
    void apply_heal(const code::client::messages::Heal& incoming);
    
    void apply_damage(std::list<code::client::messages::Damage>& incoming_list);
    void apply_damage(const code::client::messages::Damage& incoming);

protected:
    typedef std::map<std::string, std::list<std::shared_ptr<Module::HealthModule>>> HealthModuleMap;
    virtual const HealthModuleMap& test_get_health_modules() const { return m_health_module; }
    virtual const std::string& test_get_uuid_string() const { return m_uuid_string; }

private:
    HealthModuleMap m_health_module;
    std::mutex m_health_module_lock;
    bool m_initialized = false;

    float m_total_health = 0.f;
    float m_health = 0.f;

    std::string m_uuid_string;

    std::shared_ptr<core::MessagePublisher<Messages::ApplyStatus>> m_apply_status_publisher;
    std::shared_ptr<core::MessageSubscriber<Messages::ApplyDirectHeal>> m_heal_subscriber;
    std::shared_ptr<core::MessageSubscriber<Messages::ApplyDirectDamage>> m_damage_subscriber;

    std::queue<Messages::ApplyDirectHeal> m_heal_queue;
    std::queue<Messages::ApplyDirectDamage> m_damage_queue;
    std::mutex m_heal_queue_lock;
    std::mutex m_damage_queue_lock;
};
