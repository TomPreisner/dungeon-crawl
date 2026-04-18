/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include <functional>
#include <memory>
#include <string>

#include "code/core/message_system/message_dispatch_base.h"
#include "code/core/message_system/message_switchboard.h"

namespace core {

template<typename T>
class MessagePublisher {
public:
    explicit MessagePublisher(MessageSwitchboard& switchboard);
    virtual ~MessagePublisher() = default;

    bool publish_message(const T& message);
private:
    void assign_dispatcher(std::shared_ptr<MessageDispatchBase> dispatcher);
    std::weak_ptr<MessageDispatchBase> m_dispatcher;
};

///////////////////////////////////////////////////////////////////////

template<typename T>
MessagePublisher<T>::MessagePublisher(MessageSwitchboard& switchboard) {
    switchboard.register_publisher<T>([this](std::shared_ptr<MessageDispatchBase> dispatch) {
        assign_dispatcher(dispatch);
    });
}

template<typename T>
void MessagePublisher<T>::assign_dispatcher(std::shared_ptr<MessageDispatchBase> dispatcher) {
    m_dispatcher = dispatcher;
}

template<typename T>
bool MessagePublisher<T>::publish_message(const T& message) {
    if (m_dispatcher.expired()) {
        return false;
    }
    std::shared_ptr<MessageDispatchBase> dispatcher = m_dispatcher.lock();
    MessageDispatch<T>* typed_dispatch = dynamic_cast<MessageDispatch<T>*>(dispatcher.get());
    if (typed_dispatch == nullptr) {
        return false;
    }

    typed_dispatch->publish_message(message);

    return true;
}
} // namespace core 
