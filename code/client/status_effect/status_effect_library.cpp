/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/

#include "status_effect_library.h"
#include "yaml-cpp/yaml.h"

namespace Status {
    
void StatusEffectLibrary::init_from_file(const std::string& filepath) {
  YAML::Node status_library = YAML::LoadFile(filepath);

  if (status_library.Is)
}
    
    
std::optional<BaseStatusEffect> StatusEffectLibrary::get_status_effect(const std::string& status_id) {
    // If the library contains the id return the entry otherwise return nullopt
    if (m_status_library.find(status_id) != m_status_library.end()) {
        return m_status_library[status_id];
    }

    return std::nullopt;
}

} // namespace Status
