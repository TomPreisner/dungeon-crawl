/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include <string>

namespace core {

class MessageDispatchBase {
public:
    MessageDispatchBase() = default;
    virtual ~MessageDispatchBase() = default;

    virtual std::string get_id() const = 0;
    virtual bool has_subscribers() const = 0;
    virtual bool unsubscribe(const std::string& uuid_token) = 0;
};

} // namespace core 
