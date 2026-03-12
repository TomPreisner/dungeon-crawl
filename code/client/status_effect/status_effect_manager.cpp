/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/

#include "status_effect_manager.h"

#include "code/client/status_effect/status_effect_library.h"
#include "code/core/log_manager.h"

namespace Status {
CREATE_LOGGER(StatusEffectManager);

bool StatusEffectManager::apply_status_effect(const std::string& status_id) {
    if (StatusEffectLibrary::get_Instance()->has_status_effect(status_id)) {
        m_pending_status_effect_actions.emplace_back(StatusEffectAction{StatusEffectAction::APPLY, status_id, ""});
        return true;
    }
    
    LOG_WARN(StatusEffectManager, "Failed to apply status with id: " + status_id);
    return false;
}

void StatusEffectManager::clear_status_effect(const std::string& status_uuid) {
    m_pending_status_effect_actions.emplace_back(StatusEffectAction{StatusEffectAction::REMOVE, "", status_uuid});
}

void StatusEffectManager::update_manager() {
    // On the start of the update process all the pending actions
    // then call the update on the resulting list of m_status_effects
    for (const auto & action : m_pending_status_effect_actions) {
        switch(action.type) {
        case StatusEffectAction::APPLY:
            add_status_effect(action.status_id);
            break;
        case StatusEffectAction::REMOVE:
            remove_status_effect(action.status_uuid);
            break;
        }
    }
    m_pending_status_effect_actions.clear();

    // Loop through the Status Effects and update them
    for (auto & statusEffect : m_status_effects) {
        statusEffect.on_update();
    }
}

void StatusEffectManager::add_status_effect(const std::string& status_id) {
    std::optional<const StatusEffect> status_effect = 
        StatusEffectLibrary::get_Instance()->get_status_effect(status_id);

    if (!status_effect.has_value()) {
        LOG_ERROR(StatusEffectManager, "Error a pending add action exists, but the status effect isn't in the library: " + status_id);
        return;
    }

    LOG_DEBUG(StatusEffectManager, "Adding status: " + status_id + " uuid: " + status_effect->get_uuid());
    m_status_effects.emplace_back(status_effect.value());
}

void StatusEffectManager::remove_status_effect(const std::string& status_uuid) {
    const auto & end = m_status_effects.end();
    for (std::list<StatusEffect>::iterator iter = m_status_effects.begin(); iter != end; ++iter) {
        if (status_uuid == iter->get_uuid()) {
            LOG_DEBUG(StatusEffectManager, "Erasing status: " + status_uuid);
            m_status_effects.erase(iter);
            return;
        }
    }
    LOG_WARN(StatusEffectManager, "Unable to erase status effect with uuid: " + status_uuid);
}

} // namespace Status
