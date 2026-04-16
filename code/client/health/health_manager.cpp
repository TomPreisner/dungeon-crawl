/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#include "health_manager.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "code/core/log_manager.h"

CREATE_LOGGER(HealthManager);

HealthManager::HealthManager() {
    // Use the boost uuid library to create a uuid to uniquely identify this status effect instance
    boost::uuids::random_generator gen;
    boost::uuids::uuid id = gen();
    m_uuid_string = boost::uuids::to_string(id);
}

bool HealthManager::init_manager(const YAML::Node& node) {
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
        LOG_ERROR(HealthManager, "Value in \"total_health\" is not a float, init failed:\n" + YAML::Dump(amount_node));
        return false;
    }

    ////////////////////////////////////////////////////////
    const YAML::Node& health = node["health"];
    if (health) {        
        try {
            m_health = health.as<float>();
        } catch (const YAML::TypedBadConversion<float>& e) {
            LOG_ERROR(HealthManager, "Value in \"health\" is not a float, init failed:\n" + YAML::Dump(amount_node));
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
            if (!add_module(m_uuid_string, module["name"], module["data"])) {
                LOG_ERROR(HealthManager, "Bad default module detected, unable to add:\n" + YAML::Dump(module));
            }
        }
    }

    ////////////////////////////////////////////////////////
    m_initialized = true;
    return true;
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

    if (std::dynamic_cast<Module::HealthModule>(module.get()) == nullptr) {
        LOG_ERROR(HealthManager, "Requested module is not a health module: " + module_name + "  data:\n" + YAML::Dump(data));
        return false;
    }

    m_health_module[uuid_owner].emplace_back(module);
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
        if (iter->get_module_name() == module_name) {
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
    const iter = m_health_module.find(uuid_owner);
    if (iter == m_health_module.end()) {
        LOG_INFO(HealthManager, "No modules registered for uuid: " + uuid_owner);
        return true;
    }

    m_health_module.erase(iter);
    return true;
}

void HealthManager::apply_heal(std::list<Heal>& incoming_list) {
    const auto end = incoming_list.cend();
    for (const iter = incoming_list.begin(); iter != end; ++iter) {
        apply_heal(*iter);
    }
}

void HealthManager::apply_heal(const Heal& incoming) {
    // Filter the incoming heal through all of the modules
    Heal heal = incoming;

    const auto end = m_health_module.cend();
    for (auto iter = m_health_module.begin(); iter != end; ++iter) {
        const auto mod_end = iter->cend();
        for (auto mod_iter = iter->begin(); mod_iter != mod_end; ++mod_iter) {
            mod_iter->process_heal(heal);
        }
    }

    // heal the health
    if (heal.amount > 0.f) {
        delay the actual heal result until the next update loop
        m_health = std::min(m_health + heal.amount, m_total_health);
    }
    
    for (int i = 0; i < heal.status_effect.size(); ++i) {
        // apply status effects somehow?
    }
}

void HealthManager::apply_damage(std::list<Damage>& incoming_list) {
    const auto end = incoming_list.cend();
    for (const iter = incoming_list.begin(); iter != end; ++iter) {
        apply_damage(*iter);
    }
}

void HealthManager::apply_damage(const Damage& incoming) {
    // Filter the incoming heal through all of the modules
    Damage damage = incoming;

    const auto end = m_health_module.cend();
    for (auto iter = m_health_module.begin(); iter != end; ++iter) {
        const auto mod_end = iter->cend();
        for (auto mod_iter = iter->begin(); mod_iter != mod_end; ++mod_iter) {
            mod_iter->processdamage(damage);
        }
    }

    // damage the health
    if (damage.amount > 0.f) {
        delay the actual damage result until the next update loop
        m_health = std::max(m_health - damage.amount, 0.f);
    }
    
    for (int i = 0; i < damage.status_effect.size(); ++i) {
        // apply status effects somehow?
    }
}

process deferred heals before deferred damages, so if the player is close to death, they heal before taking damage that could kill them.

