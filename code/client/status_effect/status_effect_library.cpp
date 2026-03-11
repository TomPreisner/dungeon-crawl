/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/

#include "status_effect_library.h"
#include "yaml-cpp/yaml.h"

#include "code/core/log_manager.h"

namespace Status {
CREATE_LOGGER(StatusEffectLibrary);
    
void StatusEffectLibrary::init_from_file(const std::string& filepath) {
    LOG_INFO(StatusEffectLibrary, "Loading library from file: " + filepath);
    YAML::Node status_library = YAML::LoadFile(filepath);

    if (!status_library.IsMap()) {
        LOG_ERROR(StatusEffectLibrary, "Library file must contain a map at the top level: " + filepath);
        return;
    }

    const YAML::const_iterator itEnd = status_library.end();
    for (YAML::const_iterator iter = status_library.begin(); iter != itEnd; ++iter) {        
        std::string key;
        try {
            key = iter->first.as<std::string>();  
        } catch (const YAML::TypedBadConversion<std::string>& e) {
            // Not a string 
            LOG_ERROR(StatusEffectLibrary, "Key in map is not a string, skipping: " + YAML::Dump(*iter));
            continue;
        }

        if (key.empty()) {
            LOG_ERROR(StatusEffectLibrary, "Key in empty. skipping: " + YAML::Dump(*iter));
            continue;
        }

        if (m_status_library.find(key) != m_status_library.end()) {
            LOG_ERROR(StatusEffectLibrary, "Duplicate key detected : " + key + " skipping: " + YAML::Dump(*iter));
            continue;
        }

        LOG_DEBUG(StatusEffectLibrary, "Creating status effect with status_id: " + key);
        m_status_library[key] = StatusEffect(iter->second);
    }
}

std::optional<const StatusEffect> StatusEffectLibrary::get_status_effect(const std::string& status_id) {
    // If the library contains the id return the entry otherwise return nullopt
    if (m_status_library.find(status_id) != m_status_library.end()) {
        return m_status_library[status_id];
    }

    LOG_INFO(StatusEffectLibrary, "Unable to find status_id: " + status_id);
    return std::nullopt;
}

} // namespace Status
