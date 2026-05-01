/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#include "health_manager.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "code/client/modules/common/base_module.h"
#include "code/client/modules/module_factory.h"
#include "code/core/log_manager.h"

CREATE_LOGGER(HealthManager);

HealthManager::HealthManager() {
    // Use the boost uuid library to create a uuid to uniquely identify this status effect instance
    boost::uuids::random_generator gen;
    boost::uuids::uuid id = gen();
    m_uuid_string = boost::uuids::to_string(id);
}

bool HealthManager::init_manager(core::MessageSwitchboard& switchboard, const YAML::Node& node) {
    if (!node.IsMap()) {
        return false;
    }

    ////////////////////////////////////////////////////////
    const YAML::Node& total_health = node["total_health"];
    if (!total_health) {
        LOG_ERROR(HealthManager, "\"total_health\" is present, init failed:\n" + YAML::Dump(node));
        return false;
    }

    try {
        m_total_health = total_health.as<float>();
    } catch (const YAML::TypedBadConversion<float>& e) {
        LOG_ERROR(HealthManager, "Value in \"total_health\" is not a float, init failed:\n" + YAML::Dump(total_health));
        return false;
    }

    ////////////////////////////////////////////////////////
    const YAML::Node& health = node["health"];
    if (health) {
        try {
            m_health = health.as<float>();
        } catch (const YAML::TypedBadConversion<float>& e) {
            LOG_ERROR(HealthManager, "Value in \"health\" is not a float, init failed:\n" + YAML::Dump(health));
            return false;
        }
    } else {
        LOG_INFO(HealthManager, "No \"health\" is present in the init data. Using the total_health by default");
        m_health = m_total_health;
    }

    ////////////////////////////////////////////////////////
    // modules that are inherent to the character class / type
    //  These are optional
    const YAML::Node& default_modules = node["default_modules"];
    if (default_modules) {
        if (!default_modules.IsSequence()) {
            LOG_ERROR(HealthManager, "\"default_modules\" is not a sequence, skipping:\n" + YAML::Dump(default_modules));
            return false;
        }

        for (int i = 0; i < default_modules.size(); ++i) {
            const YAML::Node& module = default_modules[i];
            // the manager is the owner
            std::string module_name = "";
            try {
                module_name = module["name"].as<std::string>();
            } catch (const YAML::TypedBadConversion<std::string>& e) {
                LOG_ERROR(HealthManager, "Value in default_module \"module_name\" is not a float, init failed:\n" + YAML::Dump(module));
                continue;
            }
            if (!add_module(m_uuid_string, module_name, module["data"])) {
                LOG_ERROR(HealthManager, "Bad default module detected, unable to add:\n" + YAML::Dump(module));
            }
        }
    }

    // Create the publisher and subscribers
    m_apply_status_publisher = std::make_shared<core::MessagePublisher<Messages::ApplyStatus>>(switchboard);
    m_clear_status_publisher = std::make_shared<core::MessagePublisher<Messages::ClearStatus>>(switchboard);
    m_heal_subscriber = std::make_shared<core::MessageSubscriber<Messages::ApplyDirectHeal>>(switchboard);
    m_damage_subscriber = std::make_shared<core::MessageSubscriber<Messages::ApplyDirectDamage>>(switchboard);

    ////////////////////////////////////////////////////////
    m_initialized = true;
    return true;
}

void HealthManager::update_manager(const std::chrono::milliseconds& dt) {
    // Process deferred heals before deferred damages, so if the player is close to death, 
    //  they heal before taking damage that could kill them.
    std::queue<Messages::ApplyDirectHeal> heal_queue;
    std::queue<Messages::ApplyDirectDamage> damage_queue;
    {
        // lock the queue and move it to a local so we can process it without locking other threads
        std::scoped_lock(m_heal_queue_lock);
        std::swap(m_heal_queue, heal_queue);
    }

    {
        // lock the queue and move it to a local so we can process it without locking other threads
        std::scoped_lock(m_damage_queue_lock);
        std::swap(m_damage_queue, damage_queue);
    }

    // now that both queues are swapped, we can process the queues without worrying about things changing during operations
    while (!heal_queue.empty()) {
        float heal_amount = std::max(0.f, heal_queue.front().amount);
        m_health = std::min(m_health + heal_amount, m_total_health);
        heal_queue.pop();
    }

    while (!damage_queue.empty()) {
        float damage_amount = std::max(0.f, damage_queue.front().amount);
        m_health = std::max(m_health - damage_amount, 0.f);
        if (m_health <= 0.f) {
            // Leave the killing damage on the queue for death broadcasting
            LOG_DEBUG(HealthManager, "Death occured of " + m_uuid_string);
            break;
        }
        damage_queue.pop();
    }

    //TODO: BROADCAST DEATH
}

bool HealthManager::add_module(const std::string& uuid_owner, const std::string& module_name, const YAML::Node& data) {
    if (uuid_owner.empty()) {
        LOG_ERROR(HealthManager, "Invalid uuid_owner defined: \"" + uuid_owner + "\" for module: \"" + module_name + "\" data:\n" + YAML::Dump(data));
        return false;
    }

    if (m_health_module.find(uuid_owner) == m_health_module.end()) {
        LOG_INFO(HealthManager, "Adding modules for new uuid: " + uuid_owner);
    }

    std::shared_ptr<Module::BaseModule> module = Module::ModuleFactory::get_Instance()->create_module(module_name, data);
    if (module.get() == nullptr) {
        LOG_ERROR(HealthManager, "Failed to create module: " + module_name + " for: " + uuid_owner + "  data:\n" + YAML::Dump(data));
        return false;
    }

    Module::HealthModule* health_module_ptr = dynamic_cast<Module::HealthModule*>(module.get());
    if (health_module_ptr == nullptr) {
        LOG_ERROR(HealthManager, "Requested module is not a health module: " + module_name + "  data:\n" + YAML::Dump(data));
        return false;
    }
    // A deep copy of the module to take ownership based on a second class
    //  The previous shared pointer can NOT be moved into this one, and once this function terminates
    //  that allocated object will be deleted. So this needs to be a deep copy into new instance
    //  because if the pointer is used, it will lead to data corruption
    std::shared_ptr<Module::HealthModule> health_module = std::make_shared<Module::HealthModule>(*health_module_ptr);

    m_health_module[uuid_owner].emplace_back(health_module);
    return true;
}

bool HealthManager::remove_module(const std::string& uuid_owner, const std::string& module_name) {
    if (uuid_owner.empty() || module_name.empty()) {
        LOG_ERROR(HealthManager, "Invalid data provided for remove uuid: \"" + uuid_owner + "\" for module: \"" + module_name + "\"");
        return false;
    }

    // This is an info and early out as opposed to an error because it is technically done.
    if (m_health_module.find(uuid_owner) == m_health_module.end()) {
        LOG_INFO(HealthManager, "No modules registered for uuid: " + uuid_owner);
        return true;
    }

    auto& mod_list = m_health_module[uuid_owner];

    bool found = false;
    for (auto iter = mod_list.begin(); iter != mod_list.end();) {
        std::string iter_name = "";
        Module::BaseModule* base_module = dynamic_cast<Module::BaseModule*>(iter->get());
        if (base_module != nullptr) {
            iter_name = base_module->get_module_name();
        }
        if (!iter_name.empty() && iter_name == module_name) {
            iter = mod_list.erase(iter);
            found = true;
        } else {
            ++iter;
        }
    }

    if (!found) {
        LOG_INFO(HealthManager, "Unable to remove module: \"" + module_name + "\" for uuid: \"" + uuid_owner + "\"");
    }

    return found;
}

bool HealthManager::remove_modules_from_owner(const std::string& uuid_owner) {
    if (uuid_owner.empty()) {
        LOG_ERROR(HealthManager, "Invalid uuid provided for remove_modules_from_owner");
        return false;
    }

    // This is an info and early out as opposed to an error because it is technically done.
    const auto iter = m_health_module.find(uuid_owner);
    if (iter == m_health_module.cend()) {
        LOG_INFO(HealthManager, "No modules registered for uuid: " + uuid_owner);
        return true;
    }

    m_health_module.erase(iter);
    return true;
}

void HealthManager::apply_heal(std::list<code::client::messages::Heal>& incoming_list) {
    const auto end = incoming_list.cend();
    for (auto iter = incoming_list.begin(); iter != end; ++iter) {
        apply_heal(*iter);
    }
}

void HealthManager::apply_heal(const code::client::messages::Heal& incoming) {
    // Filter the incoming heal through all of the modules
    code::client::messages::Heal heal = incoming;

    const auto end = m_health_module.cend();
    for (auto iter = m_health_module.begin(); iter != end; ++iter) {
        const auto mod_end = iter->second.cend();
        for (auto mod_iter = iter->second.begin(); mod_iter != mod_end; ++mod_iter) {
            (*mod_iter)->process_heal(heal);
        }
    }

    // heal the health
    if (heal.amount() > 0.f) {
        //delay the actual heal result until the next update loop
        Messages::ApplyDirectHeal direct_heal = {heal.amount(), heal.type()};

        std::scoped_lock lock(m_heal_queue_lock);
        m_heal_queue.push(direct_heal);
    }

    for (int i = 0; i < heal.status_effect().size(); ++i) {
        Messages::ApplyStatus apply_status = { heal.status_effect()[i] };
        if (!m_apply_status_publisher->publish_message(apply_status)) {
            LOG_WARN(HealthManager, "Failed to publish status effect from HealthManager: " + m_uuid_string);
            break;
        }
    }
}

void HealthManager::apply_damage(std::list<code::client::messages::Damage>& incoming_list) {
    const auto end = incoming_list.cend();
    for (auto iter = incoming_list.begin(); iter != end; ++iter) {
        apply_damage(*iter);
    }
}

void HealthManager::apply_damage(const code::client::messages::Damage& incoming) {
    // Filter the incoming heal through all of the modules
    code::client::messages::Damage damage = incoming;

    const auto end = m_health_module.cend();
    for (auto iter = m_health_module.begin(); iter != end; ++iter) {
        const auto mod_end = iter->second.cend();
        for (auto mod_iter = iter->second.begin(); mod_iter != mod_end; ++mod_iter) {
            (*mod_iter)->process_damage(damage);
        }
    }

    // damage the health
    if (damage.amount() > 0.f) {
        //delay the actual damage result until the next update loop
        Messages::ApplyDirectDamage direct_damage = {damage.amount(), damage.damage_type()};

        std::scoped_lock lock(m_damage_queue_lock);
        m_damage_queue.push(direct_damage);
    }

    for (int i = 0; i < damage.status_effect().size(); ++i) {
        Messages::ApplyStatus apply_status = { damage.status_effect()[i] };
        if (!m_apply_status_publisher->publish_message(apply_status)) {
            LOG_WARN(HealthManager, "Failed to publish status effect from HealthManager: " + m_uuid_string);
            break;
        }
    }
}
