/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include <optional>

#include "code/client/status_effect/base_status_effect.h"


namespace Status {

// A library of status effects to allow easy reference of the status effects
// This is a singleton that stores all of the status effects, referenced by 
//  status id string
class StatusEffectLibrary {
public:
    static StatusEffectLibrary* get_instance() {
        static StatusEffectLibrary s_instance;
        return &s_instance;
    }

    void init_from_file(const std::string& filepath);

    std::optional<BaseStatusEffect> get_status_effect(const std::string& status_id);// { return m_status_library[status_id]; }

private:
    StatusEffectLibrary() = default;
    ~StatusEffectLibrary() = default;
    StatusEffectLibrary(const StatusEffectLibrary&) = delete;
    StatusEffectLibrary& operator=(const StatusEffectLibrary&) = delete;

    std::map<std::string, BaseStatusEffect> m_status_library;
};

} // namespace Status
