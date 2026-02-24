/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once
#include <chrono>
#include <functional>
#include <map>
#include <optional>
#include <queue>

namespace core {

void StateNoOp() { return; }
void StateUpdateNoOp(const std::chrono::milliseconds& dt) { return; }

struct State {
    std::function<void()> onEnter;
    std::function<void(const std::chrono::milliseconds& dt)> onUpdate;
    std::function<void()> onExit;
};

template<typename T>
class StateMachine {
public: 
    typedef std::map<T, State> StateMap;
    StateMachine()  {}
    ~StateMachine() {}

    void init(const T& initState, const StateMap& states);
    bool request_transition(const T& state);

    void update_tick(const std::chrono::milliseconds& dt);

    const std::queue<T>& get_history() const { return m_history; }

private:
    StateMap m_state_map;

    std::optional<T> m_curr_state;
    std::optional<T> m_next_state;
    
    std::queue<T> m_history;    // < keep a history of the state tranistions for debugging
};

template<typename T>
void StateMachine<T>::init(const T& initState, const StateMap& states) { 
    m_state_map = states;
    m_next_state = initState; 
}

template<typename T>
bool StateMachine<T>::request_transition(const T& state) {
    if (m_next_state.has_value()) {
        return false;
    }

    m_next_state = state;
    return true;
}

template<typename T>
void StateMachine<T>::update_tick(const std::chrono::milliseconds& dt) {
    if (!m_next_state.has_value()) {
        if (m_curr_state.has_value()) {
            m_state_map[m_curr_state.value()].onUpdate(dt);
        }
    } else {
        if (m_curr_state.has_value()) {
            m_state_map[m_curr_state.value()].onExit();
            m_history.push(m_curr_state.value());
        }
        m_curr_state = m_next_state;
        m_next_state.reset();
        m_state_map[m_curr_state.value()].onEnter();
    }
}

} // namespace core
