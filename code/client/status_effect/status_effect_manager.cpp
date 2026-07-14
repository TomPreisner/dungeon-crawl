/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/

#include "status_effect_manager.h"

#include "code/client/status_effect/status_effect_library.h"
#include "code/core/log_manager.h"

namespace Status {
CREATE_LOGGER(StatusEffectManager);

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
// This class is a hidden implementation that should only be used by the status effect manager class
class StatusEffectCallbackInterface_ManagerImpl : public StatusEffectCallbackInterface {
public:
    StatusEffectCallbackInterface_ManagerImpl() = default;
    ~StatusEffectCallbackInterface_ManagerImpl() = default;

    void assign_clear_callback(std::function<void(const std::string&)> clear) { m_clear_status = clear; }
    void assign_heal_callback(std::function<void(const Messages::ApplyDirectHeal&)> heal) { m_heal_event = heal; }
    void assign_damage_callback(std::function<void(const Messages::ApplyDirectDamage&)> damage) { m_damage_event = damage; }
    void assign_augment_callback(std::function<void(const std::string&)> augment) { m_augment_event = augment; }

    // No try catch for the function calls. This class is consumed only in this file, assume it won't be misused.
    virtual void cleanup_callback(const std::string& status_uuid) override {
        m_clear_status(status_uuid);
    }
    virtual void heal_callback(const std::string& status_uuid, const float amount, const code::client::messages::Heal::HealType heal_type) override {
        Messages::ApplyDirectHeal heal {amount, heal_type, status_uuid};
        m_heal_event(heal);
    }
    virtual void damage_callback(const std::string& status_uuid, const float amount, const int32_t damage_flags) override {
        Messages::ApplyDirectDamage damage {amount, damage_flags, status_uuid};
        m_damage_event(damage);
    }
    virtual void augment_callback(const std::string& status_uuid, const float amount) override {
        m_augment_event(status_uuid);
    }

private:
    // here there are function callbacks for certain functionality on the owner
    std::function<void(const std::string&)> m_clear_status;
    std::function<void(const Messages::ApplyDirectHeal&)> m_heal_event;
    std::function<void(const Messages::ApplyDirectDamage&)> m_damage_event;
    std::function<void(const std::string&)> m_augment_event;
};

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

void StatusEffectManager::init_manager(core::MessageSwitchboard& switchboard) {
    m_apply_status_subscriber = std::make_shared<core::MessageSubscriber<Messages::ApplyStatus>>(switchboard);
    m_clear_status_subscriber = std::make_shared<core::MessageSubscriber<Messages::ClearStatus>>(switchboard);
    m_heal_publisher = std::make_shared<core::MessagePublisher<Messages::ApplyDirectHeal>>(switchboard);
    m_damage_publisher = std::make_shared<core::MessagePublisher<Messages::ApplyDirectDamage>>(switchboard);

    m_apply_status_subscriber->register_callback([this](const Messages::ApplyStatus& apply_msg) {
        apply_status_effect(apply_msg.status_effect_name);
    });
    m_clear_status_subscriber->register_callback([this](const Messages::ClearStatus& clear_msg) {
        if (clear_msg.status_uuid.empty()) {
            LOG_INFO(StatusEffectManager, "Clear all Statuses requested")
            clear_all_status_effects();
        } else {
            clear_status_effect(clear_msg.status_uuid);
        }
    });

    auto callback = std::make_shared<StatusEffectCallbackInterface_ManagerImpl>();

    // register function callbacks
    callback->assign_clear_callback([this](const std::string& uuid) {
        clear_status_effect(uuid); //< enqueue a removal request
    });
    callback->assign_heal_callback([this](const Messages::ApplyDirectHeal& heal) {
        add_outgoing_message(heal); //< add an outgoing message
    });
    callback->assign_damage_callback([this](const Messages::ApplyDirectDamage& damage) {
        add_outgoing_message(damage); //< add an outgoing message
    });
    callback->assign_augment_callback([this](const std::string& uuid) {
        //Do nothing
    });

    m_callback_interface = callback;
}

bool StatusEffectManager::apply_status_effect(const std::string& status_id) {
    if (StatusEffectLibrary::get_Instance()->has_status_effect(status_id)) {
        std::scoped_lock lock(m_pending_status_effect_actions_lock);
        m_pending_status_effect_actions.push(StatusEffectAction{StatusEffectAction::APPLY, status_id, ""});
        return true;
    }
    
    LOG_WARN(StatusEffectManager, "Failed to apply status with id: " + status_id);
    return false;
}

void StatusEffectManager::clear_status_effect(const std::string& status_uuid) {
    // Since the remove is light weight, the presence of the uuids is not validated 
    //  before adding them, since the storage container is a list the find is more 
    //  costly than an incorrect removal request.
    std::scoped_lock lock(m_pending_status_effect_actions_lock);
    m_pending_status_effect_actions.push(StatusEffectAction{StatusEffectAction::REMOVE, "", status_uuid});
}

void StatusEffectManager::clear_all_status_effects() {
    std::scoped_lock lock(m_pending_status_effect_actions_lock);
    m_pending_status_effect_actions.push(StatusEffectAction{StatusEffectAction::REMOVE_ALL, "", ""});
}

void StatusEffectManager::add_outgoing_message(const Messages::ApplyDirectHeal& heal) {
    std::scoped_lock lock(m_outgoing_messages_lock);
    m_outgoing_messages.emplace_back(heal);
}

void StatusEffectManager::add_outgoing_message(const Messages::ApplyDirectDamage& damage) {
    std::scoped_lock lock(m_outgoing_messages_lock);
    m_outgoing_messages.emplace_back(damage);
}

void StatusEffectManager::update_manager(const std::chrono::milliseconds& dt) {
    // On the start of the update process all the pending actions
    // then call the update on the resulting list of m_status_effects

    std::queue<StatusEffectAction> pending_actions;
    {
        // lock the pending queue and move it to a local so we can process it without locking other threads
        std::scoped_lock(m_pending_status_effect_actions_lock);
        std::swap(m_pending_status_effect_actions, pending_actions);
    }

    // we need to lock the status effects for a while, but try to clear the lock asap
    {
        std::scoped_lock(m_status_effects_lock);
        {
            bool exit_loop = false;
            while (!pending_actions.empty()) {
                const StatusEffectAction& action = pending_actions.front();
                switch(action.type) {
                case StatusEffectAction::APPLY:
                    add_status_effect(action.status_id);
                    break;
                case StatusEffectAction::REMOVE:
                    remove_status_effect(action.status_uuid);
                    break;
                case StatusEffectAction::REMOVE_ALL:
                    // Remove all is a wipe, so processing any other status effect on the wipe
                    //  would appear as a bug.
                    remove_all_status_effects();
                    exit_loop = true;
                    break;
                }
                if (exit_loop) {
                    break;
                }
                pending_actions.pop();
            }
        }

        // Loop through the Status Effects and update them
        for (auto & statusEffect : m_status_effects) {
            statusEffect.on_update(dt);
        }
    }

    // Finally, send all of the outgoing messages. Any return effects will be dealt with next frame
    MessageList messages;
    {
        // lock the message queue and move it to a local so we can process it without locking other threads
        std::scoped_lock(m_outgoing_messages_lock);
        std::swap(m_outgoing_messages, messages);
    }

    for (auto & outmsg : messages) {
        if (std::holds_alternative<Messages::ApplyDirectHeal>(outmsg)) {
            m_heal_publisher->publish_message(std::get<Messages::ApplyDirectHeal>(outmsg));
        } else if (std::holds_alternative<Messages::ApplyDirectDamage>(outmsg)) {
            m_damage_publisher->publish_message(std::get<Messages::ApplyDirectDamage>(outmsg));
        } else {
            LOG_ERROR(StatusEffectManager, "Unknown type in outgoing message list. How did it get here?")
        }
    }
}

void StatusEffectManager::add_status_effect(const std::string& status_id) {
    std::optional<const StatusEffect> status_effect = 
        StatusEffectLibrary::get_Instance()->get_status_effect(status_id);

    if (!status_effect.has_value()) {
        LOG_ERROR(StatusEffectManager, "Error a pending add action exists, but the status effect isn't in the library: " + status_id);
        return;
    }

    std::scoped_lock(m_status_effects_lock);
    LOG_DEBUG(StatusEffectManager, "Adding status: " + status_id + " uuid: " + status_effect->get_uuid());
    m_status_effects.emplace_back(status_effect.value());
    m_status_effects.back().assign_callback_interface(m_callback_interface);
    m_status_effects.back().activate();
}

void StatusEffectManager::remove_status_effect(const std::string& status_uuid) {
    std::scoped_lock(m_status_effects_lock);
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

void StatusEffectManager::remove_all_status_effects() {
    std::scoped_lock(m_status_effects_lock);
    m_status_effects.clear();
}

} // namespace Status
