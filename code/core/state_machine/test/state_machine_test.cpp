#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include "code/core/state_machine/state_machine.h"

class StateMachine_test {
public:
    enum class StateEnum {
        STATE_1,
        STATE_2,
        STATE_3,
        STATE_4,
        STATE_5,
        STATE_6,
    };

    StateMachine_test();

    void tick(const std::chrono::milliseconds& dt) {
        m_state_machine.update_tick(dt);
    }

    const bool is_state_machine_done() const { return m_state_machine_done; }
    const std::queue<std::string>& get_status_list() const { return m_status_list; }
    const std::queue<StateEnum>& get_state_machine_history() const { return m_state_machine.get_history(); }

    static const int s_state_1_update_num = 4;
    static const int s_state_2_update_num = 3;
    static const int s_state_3_update_num = 1;
    static const int s_state_5_update_num = 2;

private:
    void State_1_OnEnter() { m_status_list.push("State_1_OnEnter"); }
    void State_1_OnUpdate(const std::chrono::milliseconds& dt) { 
        m_status_list.push("State_1_OnUpdate:" + std::to_string(dt.count()));
        if (++m_update_count >= s_state_1_update_num) {
            m_state_machine.request_transition(StateEnum::STATE_2);
            m_update_count = 0;
        }
    }
    void State_1_OnExit() { m_status_list.push("State_1_OnExit"); }

    void State_2_OnEnter() { m_status_list.push("State_2_OnEnter"); }
    void State_2_OnUpdate(const std::chrono::milliseconds& dt) { 
        m_status_list.push("State_2_OnUpdate:" + std::to_string(dt.count()));
        if (++m_update_count >= s_state_2_update_num) {
            m_state_machine.request_transition(StateEnum::STATE_4);
            m_update_count = 0;
        }
    }
    
    void State_3_OnUpdate(const std::chrono::milliseconds& dt) { 
        m_status_list.push("State_3_OnUpdate:" + std::to_string(dt.count()));
        if (++m_update_count >= s_state_3_update_num) {
            m_state_machine.request_transition(StateEnum::STATE_5);
            m_update_count = 0;
        }
    }
    void State_3_OnExit() { m_status_list.push("State_3_OnExit"); }
    
    void State_4_OnEnter() { 
        m_status_list.push("State_4_OnEnter"); 
        m_state_machine.request_transition(StateEnum::STATE_3);
    }
    void State_4_OnExit() { m_status_list.push("State_4_OnExit"); }
    
    void State_5_OnUpdate(const std::chrono::milliseconds& dt) { 
        m_status_list.push("State_5_OnUpdate:" + std::to_string(dt.count())); 
        if (++m_update_count >= s_state_5_update_num) {
            m_state_machine.request_transition(StateEnum::STATE_6);
            m_update_count = 0;
        }
    }
    
    void State_6_OnEnter() { 
        m_status_list.push("State_6_OnEnter");
        m_state_machine_done = true;
    }

    core::StateMachine<StateEnum> m_state_machine;
    std::queue<std::string> m_status_list;
    int m_update_count = 0;
    bool m_state_machine_done = false;
};

StateMachine_test::StateMachine_test(){
    std::map<StateEnum, core::State> stateMap = {
    { StateEnum::STATE_1,
      {
        [&]() { return State_1_OnEnter(); },
        [&](const std::chrono::milliseconds& dt) { return State_1_OnUpdate(dt); },
        [&]() { return State_1_OnExit(); }
      }
    },
    { StateEnum::STATE_2,
      {
        [&]() { return State_2_OnEnter(); },
        [&](const std::chrono::milliseconds& dt) { return State_2_OnUpdate(dt); },
        core::StateNoOp
      }
    },
    { StateEnum::STATE_3,
      {
        core::StateNoOp,
        [&](const std::chrono::milliseconds& dt) { return State_3_OnUpdate(dt); },
        [&]() { return State_3_OnExit(); }
      }
    },
    { StateEnum::STATE_4,
      {
        [&]() { return State_4_OnEnter(); },
        core::StateUpdateNoOp,
        [&]() { return State_4_OnExit(); }
      }
    },
    { StateEnum::STATE_5,
      {
        core::StateNoOp,
        [&](const std::chrono::milliseconds& dt) { return State_5_OnUpdate(dt); },
        core::StateNoOp
      }
    },
    { StateEnum::STATE_6,
      {
        [&]() { return State_6_OnEnter(); },
        core::StateUpdateNoOp,
        core::StateNoOp
      }
    }
    };

    m_state_machine.init(StateEnum::STATE_1, stateMap);
}

TEST(StateMachineTest, state_machine_create_test) {
    StateMachine_test state_machine_test;

    // In this test, run the state machine to completion and then
    //  validate the outputs. The tick duration is always the same 
    //  and not important to validate the class operation.
    const std::chrono::milliseconds tick_rate(10);
    while (!state_machine_test.is_state_machine_done()) {
        state_machine_test.tick(tick_rate);
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // NOTE: State 4 and State 3 are intentionally run out of order to make the test use an order
    //        that is not just the map ordering.

    std::queue<StateMachine_test::StateEnum> history = state_machine_test.get_state_machine_history();
    auto state_enum = history.front();
    EXPECT_EQ(StateMachine_test::StateEnum::STATE_1, state_enum);
    history.pop();
    
    state_enum = history.front();
    EXPECT_EQ(StateMachine_test::StateEnum::STATE_2, state_enum);
    history.pop();

    state_enum = history.front();
    EXPECT_EQ(StateMachine_test::StateEnum::STATE_4, state_enum);
    history.pop();
    
    state_enum = history.front();
    EXPECT_EQ(StateMachine_test::StateEnum::STATE_3, state_enum);
    history.pop();
    
    state_enum = history.front();
    EXPECT_EQ(StateMachine_test::StateEnum::STATE_5, state_enum);
    history.pop();
    
    // STATE_6 is the final state and the current state so it is not present in the history
    EXPECT_TRUE(history.empty());

    //////////////////////////////////////////////////////////////////////////////////////////

    std::queue<std::string> output_strings = state_machine_test.get_status_list();
    /*
    This is the expected state function call list:
State_1_OnEnter
State_1_OnUpdate:10
State_1_OnUpdate:10
State_1_OnUpdate:10
State_1_OnUpdate:10
State_1_OnExit
State_2_OnEnter
State_2_OnUpdate:10
State_2_OnUpdate:10
State_2_OnUpdate:10
State_4_OnEnter
State_4_OnExit
State_3_OnUpdate:10
State_3_OnExit
State_5_OnUpdate:10
State_5_OnUpdate:10
State_6_OnEnter

    while (!output_strings.empty()) {
        std::cout << output_strings.front() << std::endl;
        output_strings.pop();
    }
*/    
    auto val = output_strings.front();
    EXPECT_EQ("State_1_OnEnter", val);
    output_strings.pop();
    
    for (int i = 0; i < StateMachine_test::s_state_1_update_num ; ++i) {
        val = output_strings.front();
        EXPECT_EQ("State_1_OnUpdate:10", val);
        output_strings.pop();
    }
    
    val = output_strings.front();
    EXPECT_EQ("State_1_OnExit", val);
    output_strings.pop();
    
    val = output_strings.front();
    EXPECT_EQ("State_2_OnEnter", val);
    output_strings.pop();
    
    for (int i = 0; i < StateMachine_test::s_state_2_update_num ; ++i) {
        val = output_strings.front();
        EXPECT_EQ("State_2_OnUpdate:10", val);
        output_strings.pop();
    }
    
    val = output_strings.front();
    EXPECT_EQ("State_4_OnEnter", val);
    output_strings.pop();
    
    val = output_strings.front();
    EXPECT_EQ("State_4_OnExit", val);
    output_strings.pop();
    
    for (int i = 0; i < StateMachine_test::s_state_3_update_num ; ++i) {
        val = output_strings.front();
        EXPECT_EQ("State_3_OnUpdate:10", val);
        output_strings.pop();
    }
    
    val = output_strings.front();
    EXPECT_EQ("State_3_OnExit", val);
    output_strings.pop();
    
    for (int i = 0; i < StateMachine_test::s_state_5_update_num ; ++i) {
        val = output_strings.front();
        EXPECT_EQ("State_5_OnUpdate:10", val);
        output_strings.pop();
    }
    
    val = output_strings.front();
    EXPECT_EQ("State_6_OnEnter", val);
    output_strings.pop();
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
