/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include <chrono>
#include <list>
#include <mutex>
#include <variant>

#include "code/client/status_effect/status_effect.h"
#include "code/client/status_effect/status_effect_callback_interface.h"
#include "yaml-cpp/yaml.h"
#include "code/client/messages/health_messages.h"
#include "code/client/messages/status_messages.h"
#include "code/core/message_system/message_publisher.h"
#include "code/core/message_system/message_subscriber.h"
#include "code/core/message_system/message_switchboard.h"

namespace Status {

// Each character should have it's own status effect manager to manage the statuses
//  independently.  The Status' are applied through various means and a copy is stored 
// so it can be managed independently
class StatusEffectManager {
protected:
    struct StatusEffectAction {
        enum ActionType{
            APPLY,
            REMOVE,
            REMOVE_ALL
        };

        ActionType type;
        std::string status_id;
        std::string status_uuid;
    };

public:
    StatusEffectManager() {}
    ~StatusEffectManager() {}

    void init_manager(core::MessageSwitchboard& switchboard);
    void update_manager(const std::chrono::milliseconds& dt);

    bool apply_status_effect(const std::string& status_id);
    void clear_status_effect(const std::string& status_uuid);
    void clear_all_status_effects();

    void apply_heal_effects(const float amount)  {}     // TODO: Fill in
    void apply_damage_effects(const float amount) {}    // TODO: Fill in

    // Just use a variant type here to not need to do weird class heirarchy management
    using MessageList = std::list<std::variant<Messages::ApplyDirectHeal, Messages::ApplyDirectDamage>>;
protected:
    // Protected functions exposing functionality for unit tests only
    const std::list<StatusEffect>& test_get_status_effects() const { return m_status_effects; } 
    const std::queue<StatusEffectAction>& test_get_pending_status_effect_actions() const { return m_pending_status_effect_actions; }
    const MessageList& test_get_outgoing_messages() const { return m_outgoing_messages; }

private:
    void add_status_effect(const std::string& status_id);
    void remove_status_effect(const std::string& status_uuid);
    void remove_all_status_effects();
    void add_outgoing_message(const Messages::ApplyDirectHeal& heal);
    void add_outgoing_message(const Messages::ApplyDirectDamage& damage);

    //  These actions and status effects will be created and removed
    // regularly, so these are lists to allow for inser and removal easily
    std::list<StatusEffect> m_status_effects;
    std::mutex m_status_effects_lock;
    std::queue<StatusEffectAction> m_pending_status_effect_actions;
    std::mutex m_pending_status_effect_actions_lock;
    bool m_initialized = false;

    MessageList m_outgoing_messages;
    std::mutex m_outgoing_messages_lock;

    std::shared_ptr<StatusEffectCallbackInterface> m_callback_interface;

    std::shared_ptr<core::MessageSubscriber<Messages::ApplyStatus>> m_apply_status_subscriber;
    std::shared_ptr<core::MessageSubscriber<Messages::ClearStatus>> m_clear_status_subscriber;
    std::shared_ptr<core::MessagePublisher<Messages::ApplyDirectHeal>> m_heal_publisher;
    std::shared_ptr<core::MessagePublisher<Messages::ApplyDirectDamage>> m_damage_publisher;
};

} // namespace Status
