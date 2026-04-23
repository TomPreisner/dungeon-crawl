#include <gtest/gtest.h>

#include <memory>
#include <string>
#include "code/core/message_system/message_dispatch_base.h"
#include "code/core/message_system/message_subscriber.h"
#include "code/core/message_system/message_switchboard.h"
#include "code/core/message_system/test/data/message_test_data.h"

template<typename T>
class MessageSubscriberTest : public core::MessageSubscriber<T> {
public:
    explicit MessageSubscriberTest(core::MessageSwitchboard& switchboard) : core::MessageSubscriber<T>(switchboard) {}
    std::weak_ptr<core::MessageDispatchBase> public_test_get_dispatcher() const { return test_get_dispatcher(); }
    std::string public_test_get_uuid_token() const { return test_get_uuid_token(); }
};

TEST(MessageSubscriber_Test, MessageSubscriber_Simple) {
    std::shared_ptr<core::MessageSwitchboard> switchboard = std::make_shared<core::MessageSwitchboard>();
    EXPECT_FALSE(switchboard.get() == nullptr);

    int sub_1_calls = 0;
    int sub_2_calls = 0;

    std::shared_ptr<MessageSubscriberTest<SimpleData>> sub_1 = 
      std::make_shared<MessageSubscriberTest<SimpleData>>(*switchboard);
    EXPECT_TRUE(sub_1->register_callback([&sub_1_calls](const SimpleData& msg) {
        ++sub_1_calls;
    }));
    EXPECT_FALSE(sub_1->public_test_get_dispatcher().expired());
    EXPECT_EQ(sub_1->public_test_get_dispatcher().use_count(), 1);
    EXPECT_TRUE(sub_1->public_test_get_dispatcher().lock()->has_subscribers());

    std::shared_ptr<MessageSubscriberTest<SimpleData>> sub_2 = 
      std::make_shared<MessageSubscriberTest<SimpleData>>(*switchboard);
    EXPECT_TRUE(sub_2->register_callback([&sub_2_calls](const SimpleData& msg) {
        ++sub_2_calls;
    }));
    EXPECT_FALSE(sub_2->public_test_get_dispatcher().expired());
    EXPECT_EQ(sub_2->public_test_get_dispatcher().use_count(), 1);
    EXPECT_TRUE(sub_2->public_test_get_dispatcher().lock()->has_subscribers());

    SimpleData test_data = {7654321, "fooBar"};
    switchboard->publish_message<SimpleData>(test_data);
    EXPECT_EQ(sub_1_calls, 1);
    EXPECT_EQ(sub_2_calls, 1);

    EXPECT_TRUE(sub_1->public_test_get_dispatcher().lock()->unsubscribe(sub_1->public_test_get_uuid_token()));

    switchboard->publish_message<SimpleData>(test_data);
    EXPECT_EQ(sub_1_calls, 1);
    EXPECT_EQ(sub_2_calls, 2);

    EXPECT_FALSE(sub_1->public_test_get_dispatcher().lock()->unsubscribe(sub_1->public_test_get_uuid_token()));

    switchboard->publish_message<SimpleData>(test_data);
    EXPECT_EQ(sub_1_calls, 1);
    EXPECT_EQ(sub_2_calls, 3);

    // re-regisiter
    EXPECT_TRUE(sub_1->register_callback([&sub_1_calls](const SimpleData& msg) {
          ++sub_1_calls;
    }));
    EXPECT_FALSE(sub_2->public_test_get_dispatcher().expired());
    EXPECT_EQ(sub_2->public_test_get_dispatcher().use_count(), 1);

    switchboard->publish_message<SimpleData>(test_data);
    EXPECT_EQ(sub_1_calls, 2);
    EXPECT_EQ(sub_2_calls, 4);

    sub_2.reset();
    EXPECT_TRUE(sub_2.get() == nullptr);

    switchboard->publish_message<SimpleData>(test_data);
    EXPECT_EQ(sub_1_calls, 3);
    EXPECT_EQ(sub_2_calls, 4);

    // now delete the switchboard and make sure that the publisher doesn't crash
    switchboard.reset();
    EXPECT_TRUE(switchboard.get() == nullptr);

    EXPECT_TRUE(sub_1->public_test_get_dispatcher().expired());
    EXPECT_EQ(sub_1->public_test_get_dispatcher().use_count(), 0);

    EXPECT_FALSE(sub_1->unregister());
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
