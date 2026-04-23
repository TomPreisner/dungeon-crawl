/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include <functional>
#include <memory>
#include <string>

#include "code/core/message_system/message_dispatch_base.h"
#include "code/core/message_system/message_pub_sub_base.h"
#include "code/core/message_system/message_switchboard.h"

namespace core {

template<typename T>
class MessageSubscriber : public MessagePubSubBase {
public:
    explicit MessageSubscriber(MessageSwitchboard& switchboard);
    virtual ~MessageSubscriber();

    bool register_callback(std::function<void(const T&)> callback);
    bool unregister();

protected:
    std::weak_ptr<MessageDispatchBase> test_get_dispatcher() const { return m_dispatcher; }
    std::string test_get_uuid_token() const { return m_uuid_token; }

private:
    std::weak_ptr<MessageDispatchBase> m_dispatcher;
    std::string m_uuid_token;
};

///////////////////////////////////////////////////////////////////////

template<typename T>
MessageSubscriber<T>::MessageSubscriber(MessageSwitchboard& switchboard) {
    request_dispatcher<T>(switchboard, [this](std::shared_ptr<MessageDispatchBase> dispatch) {
        m_dispatcher = dispatch;
    });
}

template<typename T>
MessageSubscriber<T>::~MessageSubscriber() {
    unregister();
}

template<typename T>
bool MessageSubscriber<T>::register_callback(std::function<void(const T&)> callback) {
    if (m_dispatcher.expired()) {
        return false;
    }
    std::shared_ptr<MessageDispatchBase> dispatcher = m_dispatcher.lock();
    auto typed_dispatch = dynamic_cast<MessageDispatch<T>*>(dispatcher.get());
    if (typed_dispatch == nullptr) {
        return false;
    }

    if (m_uuid_token.empty()) {
        m_uuid_token = typed_dispatch->subscribe_to_message(callback);
    } else {
        typed_dispatch->subscribe_to_message(m_uuid_token, callback);
    }

    return !m_uuid_token.empty();
}

template<typename T>
bool MessageSubscriber<T>::unregister() {
    if (m_dispatcher.expired()) {
        return false;
    }
    std::shared_ptr<MessageDispatchBase> dispatcher = m_dispatcher.lock();
    return dispatcher->unsubscribe(m_uuid_token);
}

} // namespace core 
