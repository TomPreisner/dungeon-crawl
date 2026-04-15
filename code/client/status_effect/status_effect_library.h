/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include <map>
#include <optional>

#include "code/client/status_effect/status_effect.h"

namespace Status {

// A library of status effects to allow easy reference of the status effects
// This is a singleton that stores all of the status effects, referenced by 
//  status id string
class StatusEffectLibrary final {
private:
    // The passkey pattern is necessary here to use the the unique_ptr without
    //  exposing a constructor that other classes can use to create an instance
    class StatusEffectLibraryPasskey {
     private:
        friend StatusEffectLibrary;
        StatusEffectLibraryPasskey() = default;
        ~StatusEffectLibraryPasskey() = default;
    };

public:
    StatusEffectLibrary(StatusEffectLibraryPasskey passkey) : StatusEffectLibrary() { }
    ~StatusEffectLibrary() = default;

    static StatusEffectLibrary* get_Instance() {
        static StatusEffectLibrary::StatusEffectLibraryPasskey s_passkey;
        static std::unique_ptr<StatusEffectLibrary> s_instance = std::make_unique<StatusEffectLibrary>(s_passkey);
        return s_instance.get();
    }

    void init_from_file(const std::string& filepath);
    void shutdown();

    bool has_status_effect(const std::string& status_id);
    std::optional<const StatusEffect> get_status_effect(const std::string& status_id);

private:
    StatusEffectLibrary() = default;
    StatusEffectLibrary(const StatusEffectLibrary&) = delete;
    StatusEffectLibrary& operator=(const StatusEffectLibrary&) = delete;

    std::map<std::string, StatusEffect> m_status_library;
};

} // namespace Status
