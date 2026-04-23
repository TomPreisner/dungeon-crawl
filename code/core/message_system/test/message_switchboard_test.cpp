#include <gtest/gtest.h>

#include <memory>
#include <string>
#include "code/core/message_system/message_dispatch_base.h"
#include "code/core/message_system/message_switchboard.h"
#include "code/core/message_system/test/data/message_test_data.h"

class MessageSwitchboardTest : public core::MessageSwitchboard {
public:
    std::map<std::string, std::shared_ptr<core::MessageDispatchBase>>& public_test_get_dispatchers() { return test_get_dispatchers(); }
    
    template<typename T>
    bool public_test_dispatcher_callback(
          std::function<void(std::shared_ptr<core::MessageDispatchBase>)> callback) 
      { return test_dispatcher_callback<T>(callback); }
};

TEST(MessageSwitchboard_Test, MessageSwitchboard_Simple) {
    int simple_count = 0;
    int complex_count = 0;

    MessageSwitchboardTest switchboard;
    EXPECT_EQ(switchboard.public_test_get_dispatchers().size(), 0);

    SimpleData simple = {7654321, "fooBar"};

    SimpleData test_data = {7654321, "fooBar"};
    SimpleData test_data_two = {12345, "barFoo"};
    SimpleData test_data_three = {567890, "testFoo"};
    ComplexData complex;
    complex.uuid = "6fa459ea-ee8a-3ca4-894e-db77e160355e";
    complex.simple_vector.push_back(test_data);
    complex.simple_vector.push_back(test_data_two);
    complex.simple_vector.push_back(test_data_three);
    complex.mapping[ComplexData::EnumType::VAL_ONE] = "apple";
    complex.mapping[ComplexData::EnumType::VAL_TWO] = "banana";
    complex.mapping[ComplexData::EnumType::VAL_THREE] = "pear";

    EXPECT_NE(core::MessageDispatch<SimpleData>::get_dispatch_id(),
              core::MessageDispatch<ComplexData>::get_dispatch_id());

    EXPECT_FALSE(switchboard.publish_message<SimpleData>(simple));
    EXPECT_FALSE(switchboard.publish_message<ComplexData>(complex));

    // Add a callback, make sure a dispatcher is allocated
    EXPECT_TRUE(switchboard.public_test_dispatcher_callback<SimpleData>([&simple_count](std::shared_ptr<core::MessageDispatchBase>) {
      ++simple_count;
    }));
    EXPECT_EQ(simple_count, 1);
    EXPECT_EQ(complex_count, 0);

    EXPECT_EQ(switchboard.public_test_get_dispatchers().size(), 1);
    EXPECT_TRUE(switchboard.public_test_get_dispatchers().find(core::MessageDispatch<SimpleData>::get_dispatch_id()) != 
                switchboard.public_test_get_dispatchers().end());
    EXPECT_TRUE(switchboard.public_test_get_dispatchers().find(core::MessageDispatch<ComplexData>::get_dispatch_id()) == 
                switchboard.public_test_get_dispatchers().end());

    EXPECT_TRUE(switchboard.publish_message<SimpleData>(simple));
    EXPECT_FALSE(switchboard.publish_message<ComplexData>(complex));

    // Add a callback on the other message, make sure there are two now
    EXPECT_TRUE(switchboard.public_test_dispatcher_callback<ComplexData>([&complex_count](std::shared_ptr<core::MessageDispatchBase>) {
      ++complex_count;
    }));
    EXPECT_EQ(simple_count, 1);
    EXPECT_EQ(complex_count, 1);

    EXPECT_EQ(switchboard.public_test_get_dispatchers().size(), 2);
    EXPECT_TRUE(switchboard.public_test_get_dispatchers().find(core::MessageDispatch<SimpleData>::get_dispatch_id()) != 
                switchboard.public_test_get_dispatchers().end());
    EXPECT_TRUE(switchboard.public_test_get_dispatchers().find(core::MessageDispatch<ComplexData>::get_dispatch_id()) != 
                switchboard.public_test_get_dispatchers().end());

    EXPECT_TRUE(switchboard.publish_message<SimpleData>(simple));
    EXPECT_TRUE(switchboard.publish_message<ComplexData>(complex));

    // validate no cross pollution occurs
    EXPECT_EQ(simple_count, 1);
    EXPECT_EQ(complex_count, 1);

    // reset dispatchers
    switchboard.public_test_get_dispatchers().clear();

    EXPECT_FALSE(switchboard.publish_message<SimpleData>(simple));
    EXPECT_FALSE(switchboard.publish_message<ComplexData>(complex));

    // reset dispatchers
    switchboard.public_test_get_dispatchers().clear();

    EXPECT_FALSE(switchboard.publish_message<SimpleData>(simple));
    EXPECT_FALSE(switchboard.publish_message<ComplexData>(complex));
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
