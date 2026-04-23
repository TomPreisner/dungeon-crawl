/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include <functional>
#include <memory>

#include "code/core/message_system/message_dispatch_base.h"
#include "code/core/message_system/message_switchboard.h"

namespace core {

class MessagePubSubBase {
protected:
    template<typename T>
    bool request_dispatcher(MessageSwitchboard& switchboard, std::function<void(std::shared_ptr<MessageDispatchBase>)> callback) {
        return switchboard.dispatcher_callback<T>(callback);
    }
};

} // namespace core 
