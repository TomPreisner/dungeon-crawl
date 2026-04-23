#include <gtest/gtest.h>

#include <memory>
#include <string>
#include "code/core/message_system/message_publisher.h"
#include "code/core/message_system/message_subscriber.h"
#include "code/core/message_system/message_switchboard.h"
#include "code/core/message_system/test/data/message_system_test.pb.h"

TEST(MessageSystem_Test, MessageSystem_Simple) {
    core::MessageSwitchboard switchboard;

    int person_calls = 0;
    int address_calls = 0;
    
    core::MessagePublisher<code::core::test::data::Person> person_pub(switchboard);    
    core::MessagePublisher<code::core::test::data::AddressBook> address_pub(switchboard);

    core::MessageSubscriber<code::core::test::data::Person> person_sub(switchboard);
    EXPECT_TRUE(person_sub.register_callback([&person_calls](const code::core::test::data::Person& person_msg) {
        ++person_calls;
    }));
    core::MessageSubscriber<code::core::test::data::AddressBook> address_sub(switchboard);
    EXPECT_TRUE(address_sub.register_callback([&address_calls](const code::core::test::data::AddressBook& address_msg) {
        ++address_calls;
    }));

    code::core::test::data::Person person_msg;
    code::core::test::data::AddressBook address_msg;

    EXPECT_TRUE(person_pub.publish_message(person_msg));
    EXPECT_TRUE(address_pub.publish_message(address_msg));
    EXPECT_TRUE(person_pub.publish_message(person_msg));

    EXPECT_EQ(person_calls, 2);
    EXPECT_EQ(address_calls, 1);
}

TEST(MessageSystem_Test, MessageSystem_MultipleSwitchboards) {
    // This validates that the switchboards are isolated from one another
    //  This is a concern with templates as it is possible to accidently create 
    //     static templated member variables that polute other instances.
    core::MessageSwitchboard switchboard[3];
    int person_calls[3] = {0, 0, 0};
    int address_calls[3] = {0, 0, 0};

    core::MessagePublisher<code::core::test::data::Person> person_pub_0(switchboard[0]);    
    core::MessagePublisher<code::core::test::data::AddressBook> address_pub_0(switchboard[0]);
    core::MessagePublisher<code::core::test::data::Person> person_pub_1(switchboard[1]);    
    core::MessagePublisher<code::core::test::data::AddressBook> address_pub_1(switchboard[1]);
    core::MessagePublisher<code::core::test::data::Person> person_pub_2(switchboard[2]);    
    core::MessagePublisher<code::core::test::data::AddressBook> address_pub_2(switchboard[2]);

    core::MessageSubscriber<code::core::test::data::Person> person_sub_0(switchboard[0]);
    EXPECT_TRUE(person_sub_0.register_callback([&](const code::core::test::data::Person& person_msg) {
        ++person_calls[0];
    }));
    core::MessageSubscriber<code::core::test::data::Person> person_sub_1(switchboard[1]);
    EXPECT_TRUE(person_sub_1.register_callback([&](const code::core::test::data::Person& person_msg) {
        ++person_calls[1];
    }));
    core::MessageSubscriber<code::core::test::data::Person> person_sub_2(switchboard[2]);
    EXPECT_TRUE(person_sub_2.register_callback([&](const code::core::test::data::Person& person_msg) {
        ++person_calls[2];
    }));
    core::MessageSubscriber<code::core::test::data::AddressBook> address_sub_0(switchboard[0]);
    EXPECT_TRUE(address_sub_0.register_callback([&](const code::core::test::data::AddressBook& address_msg) {
        ++address_calls[0];
    }));
    core::MessageSubscriber<code::core::test::data::AddressBook> address_sub_1(switchboard[1]);
    EXPECT_TRUE(address_sub_1.register_callback([&](const code::core::test::data::AddressBook& address_msg) {
        ++address_calls[1];
    }));
    core::MessageSubscriber<code::core::test::data::AddressBook> address_sub_2(switchboard[2]);
    EXPECT_TRUE(address_sub_2.register_callback([&](const code::core::test::data::AddressBook& address_msg) {
        ++address_calls[2];
    }));

    code::core::test::data::Person person_msg;
    code::core::test::data::AddressBook address_msg;

    EXPECT_TRUE(person_pub_0.publish_message(person_msg));
    EXPECT_TRUE(address_pub_0.publish_message(address_msg));
    EXPECT_TRUE(person_pub_0.publish_message(person_msg));

    EXPECT_TRUE(address_pub_1.publish_message(address_msg));
    EXPECT_TRUE(person_pub_1.publish_message(person_msg));
    EXPECT_TRUE(address_pub_1.publish_message(address_msg));
    EXPECT_TRUE(address_pub_1.publish_message(address_msg));

    EXPECT_TRUE(person_pub_2.publish_message(person_msg));
    EXPECT_TRUE(person_pub_2.publish_message(person_msg));
    EXPECT_TRUE(address_pub_2.publish_message(address_msg));
    EXPECT_TRUE(person_pub_2.publish_message(person_msg));
    EXPECT_TRUE(address_pub_2.publish_message(address_msg));
    EXPECT_TRUE(person_pub_2.publish_message(person_msg));
    
    EXPECT_EQ(person_calls[0], 2);
    EXPECT_EQ(address_calls[0], 1);
    EXPECT_EQ(person_calls[1], 1);
    EXPECT_EQ(address_calls[1], 3);
    EXPECT_EQ(person_calls[2], 4);
    EXPECT_EQ(address_calls[2], 2);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
