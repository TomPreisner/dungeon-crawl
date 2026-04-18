/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <typeinfo>
#include <vector>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "code/core/message_system/message_dispatch_base.h"

namespace core {
template<typename T>
class MessageDispatch : public MessageDispatchBase {
public:
    MessageDispatch() = default;
    virtual ~MessageDispatch(); // we need to wait on the lock to destruct

    // Use RTTI to create the identifier for this type
    inline static std::string get_dispatch_id() {
        if (m_indentifier.empty()) {
            MessageDispatch<T> dispatcher;
            m_indentifier = typeid(dispatcher).name();
        }
        return m_indentifier;
    }

    virtual std::string get_id() const override { return get_dispatch_id(); };
    virtual bool has_subscribers() const override { return !m_subscribers.empty(); };

    void publish_message(const T& message);

    std::string subscribe_to_message(std::function<void(const T&)> callback); // return a uuid to the subscriber
    void subscribe_to_message(const std::string& uuid_token, std::function<void(const T&)> callback); // return a uuid to the subscriber
    virtual bool unsubscribe(const std::string& uuid_token) override;
private:
    std::mutex m_lock;
    std::map<std::string, std::function<void(const T&)>> m_subscribers; // The string is the uuid_token
    static inline std::string m_indentifier;
};

///////////////////////////////////////////////////////////////////////
template<typename T>
MessageDispatch<T>::~MessageDispatch() {
    // get the lock to prevent clearing the subscibers as they get operated on
    std::scoped_lock lock(m_lock);
    m_subscribers.clear();
}

template<typename T>
void MessageDispatch<T>::publish_message(const T& message) {
    std::scoped_lock lock(m_lock);

    const auto end = m_subscribers.cend();
    for (auto iter = m_subscribers.begin(); iter != end; ++iter) {
        iter->second(message);
    }
}

template<typename T>
std::string MessageDispatch<T>::subscribe_to_message(std::function<void(const T&)> callback) {
    boost::uuids::random_generator gen;
    boost::uuids::uuid id = gen();
    std::string uuid_string = boost::uuids::to_string(id);

    subscribe_to_message(uuid_string, callback);

    return uuid_string;
}

template<typename T>
void MessageDispatch<T>::subscribe_to_message(const std::string& uuid_string, std::function<void(const T&)> callback) {
    std::scoped_lock lock(m_lock);
    m_subscribers[uuid_string] = callback;
}

template<typename T>
bool MessageDispatch<T>::unsubscribe(const std::string& uuid) {
    std::scoped_lock lock(m_lock);
    return m_subscribers.erase(uuid) > 0;
}

} // namespace core 
