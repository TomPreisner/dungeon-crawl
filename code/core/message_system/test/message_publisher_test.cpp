#include <gtest/gtest.h>

#include <memory>
#include <string>
#include "code/core/message_system/message_dispatch_base.h"
#include "code/core/message_system/message_publisher.h"
#include "code/core/message_system/message_switchboard.h"
#include "code/core/message_system/test/data/message_test_data.h"

template<typename T>
class MessagePublisherTest : public core::MessagePublisher<T> {
public:
    explicit MessagePublisherTest(core::MessageSwitchboard& switchboard) : core::MessagePublisher<T>(switchboard) {}
    std::weak_ptr<core::MessageDispatchBase> public_test_get_dispatcher() const { return test_get_dispatcher(); }
};

TEST(MessagePublisher_Test, MessagePublisher_Simple) {
    std::shared_ptr<core::MessageSwitchboard> switchboard = std::make_shared<core::MessageSwitchboard>();
    EXPECT_FALSE(switchboard.get() == nullptr);

    SimpleData test_data = {7654321, "fooBar"};
    MessagePublisherTest<SimpleData> pub_one(*switchboard);
    EXPECT_FALSE(pub_one.public_test_get_dispatcher().expired());
    EXPECT_EQ(pub_one.public_test_get_dispatcher().use_count(), 1);
    EXPECT_TRUE(pub_one.publish_message(test_data));

    // now delete the switchboard and make sure that the publisher doesn't crash
    switchboard.reset();
    EXPECT_TRUE(switchboard.get() == nullptr);

    EXPECT_TRUE(pub_one.public_test_get_dispatcher().expired());
    EXPECT_EQ(pub_one.public_test_get_dispatcher().use_count(), 0);
    EXPECT_FALSE(pub_one.publish_message(test_data));
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
