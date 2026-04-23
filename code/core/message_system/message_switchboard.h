/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include <functional>
#include <string>
#include <mutex>
#include <memory>
#include <map>

#include "code/core/message_system/message_dispatch_base.h"
#include "code/core/message_system/message_dispatch.h"

namespace core {
///////////////////////////////////////////////////////////////////////
class MessageSwitchboard {
public:
    template<typename T> 
    bool publish_message(const T& message);

    template<typename T>
    bool register_publisher(std::function<void(std::shared_ptr<MessageDispatchBase>)> register_callback);
    template<typename T>
    bool register_subscriber(std::function<void(std::shared_ptr<MessageDispatchBase>)> register_callback);

protected:
    std::map<std::string, std::shared_ptr<MessageDispatchBase>>& test_get_dispatchers() { return m_dispatchers; }

private:
    template<typename T>
    bool dispatcher_callback(std::function<void(std::shared_ptr<MessageDispatchBase>)> callback);

    std::mutex m_lock;
    // indexed map based on the dispatcher id.
    std::map<std::string, std::shared_ptr<MessageDispatchBase>> m_dispatchers;
};

///////////////////////////////////////////////////////////////////////
template<typename T> 
bool MessageSwitchboard::publish_message(const T& message) {
    std::string id = MessageDispatch<T>::get_dispatch_id();

    std::scoped_lock lock(m_lock);
    // dispatchers are allocated when a subscriber registers for a message type
    //  if there are no subscribers, there is no reason to publish messages
    if (m_dispatchers.find(id) == m_dispatchers.end()) {
        return false;
    }

    auto dispatch = dynamic_cast<MessageDispatch<T>*>(m_dispatchers[id].get());
    if (dispatch == nullptr) {
        return false;
    }

    dispatch->publish_message(message);
    return true;
}

///////////////////////////////////////////////////////////////////////
template<typename T>
bool MessageSwitchboard::register_publisher(std::function<void(std::shared_ptr<MessageDispatchBase>)> register_callback) {
    return dispatcher_callback<T>(register_callback);
}

///////////////////////////////////////////////////////////////////////
template<typename T>
bool MessageSwitchboard::register_subscriber(std::function<void(std::shared_ptr<MessageDispatchBase>)> register_callback) {
    return dispatcher_callback<T>(register_callback);
}

///////////////////////////////////////////////////////////////////////
template<typename T>
bool MessageSwitchboard::dispatcher_callback(std::function<void(std::shared_ptr<MessageDispatchBase>)> callback) {
    // if the dispatcher does not exist create it.
    std::string id = MessageDispatch<T>::get_dispatch_id();

    std::scoped_lock lock(m_lock);
    if (m_dispatchers[id].get() == nullptr) {
        m_dispatchers[id] = std::make_shared<MessageDispatch<T>>();
    }

    callback(m_dispatchers[id]);

    return true;
}
} // namespace core 
