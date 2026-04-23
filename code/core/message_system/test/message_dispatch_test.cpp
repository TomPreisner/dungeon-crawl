#include <gtest/gtest.h>

#include <memory>
#include <vector>
#include <string>
#include "code/core/message_system/message_dispatch.h"
#include "code/core/message_system/test/data/message_system_test.pb.h"
#include "code/core/message_system/test/data/message_test_data.h"

template<typename T>
class MessageDispatchTest : public core::MessageDispatch<T> {
public:
    std::map<std::string, std::function<void(const T&)>> public_test_get_subscribers() const { return test_get_subscribers(); }
};

TEST(MessageDispatch_Test, MessageDispatch_Simple) {
    MessageDispatchTest<SimpleData> message_dispatch_simple;
    std::string sub_uuid;
    int call_count = 0;

    EXPECT_EQ(MessageDispatchTest<SimpleData>::get_dispatch_id(), message_dispatch_simple.get_id());
    EXPECT_EQ(MessageDispatchTest<SimpleData>::get_dispatch_id(), "class core::MessageDispatch<struct SimpleData>");
    EXPECT_FALSE(message_dispatch_simple.has_subscribers());

    sub_uuid = message_dispatch_simple.subscribe_to_message([&call_count](const SimpleData& data) {
        EXPECT_EQ(data.value, 7654321);
        EXPECT_EQ(data.name, "fooBar");
        ++call_count;
    });
    EXPECT_TRUE(message_dispatch_simple.has_subscribers());
    EXPECT_EQ(message_dispatch_simple.public_test_get_subscribers().size(), 1);

    SimpleData test_data = {7654321, "fooBar"};
    message_dispatch_simple.publish_message(test_data);

    EXPECT_FALSE(message_dispatch_simple.unsubscribe("INVALID UUID VALUE"));
    EXPECT_TRUE(message_dispatch_simple.has_subscribers());
    EXPECT_EQ(message_dispatch_simple.public_test_get_subscribers().size(), 1);
    EXPECT_TRUE(message_dispatch_simple.unsubscribe(sub_uuid));
    EXPECT_FALSE(message_dispatch_simple.has_subscribers());
    EXPECT_EQ(message_dispatch_simple.public_test_get_subscribers().size(), 0);
    
    SimpleData test_data_two = {12345, "barFoo"};
    message_dispatch_simple.publish_message(test_data_two);

    EXPECT_EQ(call_count, 1);
}

TEST(MessageDispatch_Test, MessageDispatch_Simple_SubscriberTest) {
    MessageDispatchTest<SimpleData> message_dispatch_simple;
    std::string sub_uuids[3] = {"","",""};
    int callback_count[3] = {0,0,0};

    EXPECT_EQ(MessageDispatchTest<SimpleData>::get_dispatch_id(), message_dispatch_simple.get_id());
    EXPECT_EQ(MessageDispatchTest<SimpleData>::get_dispatch_id(), "class core::MessageDispatch<struct SimpleData>");
    EXPECT_FALSE(message_dispatch_simple.has_subscribers());

    sub_uuids[0] = message_dispatch_simple.subscribe_to_message([&](const SimpleData& data) {
        ++callback_count[0];
    });
    sub_uuids[1] = message_dispatch_simple.subscribe_to_message([&](const SimpleData& data) {
        ++callback_count[1];
    });
    sub_uuids[2] = message_dispatch_simple.subscribe_to_message([&](const SimpleData& data) {
        ++callback_count[2];
    });
    EXPECT_TRUE(message_dispatch_simple.has_subscribers());
    EXPECT_EQ(message_dispatch_simple.public_test_get_subscribers().size(), 3);

    SimpleData test_data = {7654321, "fooBar"};
    message_dispatch_simple.publish_message(test_data);

    EXPECT_EQ(callback_count[0], 1);
    EXPECT_EQ(callback_count[1], 1);
    EXPECT_EQ(callback_count[2], 1);

    EXPECT_TRUE(message_dispatch_simple.unsubscribe(sub_uuids[1]));
    EXPECT_TRUE(message_dispatch_simple.has_subscribers());
    EXPECT_EQ(message_dispatch_simple.public_test_get_subscribers().size(), 2);

    SimpleData test_data_two = {12345, "barFoo"};
    message_dispatch_simple.publish_message(test_data_two);

    EXPECT_EQ(callback_count[0], 2);
    EXPECT_EQ(callback_count[1], 1);
    EXPECT_EQ(callback_count[2], 2);

    EXPECT_FALSE(message_dispatch_simple.unsubscribe(sub_uuids[1]));
    EXPECT_TRUE(message_dispatch_simple.has_subscribers());
    EXPECT_EQ(message_dispatch_simple.public_test_get_subscribers().size(), 2);

    EXPECT_TRUE(message_dispatch_simple.unsubscribe(sub_uuids[0]));
    EXPECT_TRUE(message_dispatch_simple.has_subscribers());
    EXPECT_EQ(message_dispatch_simple.public_test_get_subscribers().size(), 1);

    SimpleData test_data_three = {567890, "testFoo"};
    message_dispatch_simple.publish_message(test_data_three);

    EXPECT_EQ(callback_count[0], 2);
    EXPECT_EQ(callback_count[1], 1);
    EXPECT_EQ(callback_count[2], 3);

    message_dispatch_simple.subscribe_to_message(sub_uuids[1], [&](const SimpleData& data) {
        ++callback_count[1];
    });
    EXPECT_TRUE(message_dispatch_simple.has_subscribers());
    EXPECT_EQ(message_dispatch_simple.public_test_get_subscribers().size(), 2);

    message_dispatch_simple.publish_message(test_data);
    message_dispatch_simple.publish_message(test_data_two);
    message_dispatch_simple.publish_message(test_data_three);

    EXPECT_EQ(callback_count[0], 2);
    EXPECT_EQ(callback_count[1], 4);
    EXPECT_EQ(callback_count[2], 6);

    EXPECT_TRUE(message_dispatch_simple.unsubscribe(sub_uuids[1]));
    EXPECT_TRUE(message_dispatch_simple.has_subscribers());
    EXPECT_EQ(message_dispatch_simple.public_test_get_subscribers().size(), 1);

    message_dispatch_simple.publish_message(test_data);

    EXPECT_EQ(callback_count[0], 2);
    EXPECT_EQ(callback_count[1], 4);
    EXPECT_EQ(callback_count[2], 7);
}

TEST(MessageDispatch_Test, MessageDispatch_Complex) {
    MessageDispatchTest<ComplexData> message_dispatch_complex;
    std::string sub_uuid;
    int call_count= 0;

    EXPECT_EQ(MessageDispatchTest<ComplexData>::get_dispatch_id(), message_dispatch_complex.get_id());
    EXPECT_EQ(MessageDispatchTest<ComplexData>::get_dispatch_id(), "class core::MessageDispatch<struct ComplexData>");
    EXPECT_FALSE(message_dispatch_complex.has_subscribers());

    sub_uuid = message_dispatch_complex.subscribe_to_message([&call_count](const ComplexData& data) {
        EXPECT_EQ(data.uuid, "6fa459ea-ee8a-3ca4-894e-db77e160355e");
        EXPECT_EQ(data.simple_vector.size(), 3);
        EXPECT_EQ(data.simple_vector[0].value, 7654321);
        EXPECT_EQ(data.simple_vector[0].name, "fooBar");
        EXPECT_EQ(data.simple_vector[1].value, 12345);
        EXPECT_EQ(data.simple_vector[1].name, "barFoo");
        EXPECT_EQ(data.simple_vector[2].value, 567890);
        EXPECT_EQ(data.simple_vector[2].name, "testFoo");

        EXPECT_EQ(data.mapping.size(), 3);
        EXPECT_TRUE(data.mapping.find(ComplexData::EnumType::VAL_ONE) != data.mapping.end());
        EXPECT_EQ(data.mapping.at(ComplexData::EnumType::VAL_ONE), "apple");
        EXPECT_TRUE(data.mapping.find(ComplexData::EnumType::VAL_TWO) != data.mapping.end());
        EXPECT_EQ(data.mapping.at(ComplexData::EnumType::VAL_TWO), "banana");
        EXPECT_TRUE(data.mapping.find(ComplexData::EnumType::VAL_THREE) != data.mapping.end());
        EXPECT_EQ(data.mapping.at(ComplexData::EnumType::VAL_THREE), "pear");

        ++call_count;
    });
    EXPECT_TRUE(message_dispatch_complex.has_subscribers());
    EXPECT_EQ(message_dispatch_complex.public_test_get_subscribers().size(), 1);

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
    message_dispatch_complex.publish_message(complex);

    EXPECT_FALSE(message_dispatch_complex.unsubscribe("INVALID UUID VALUE"));
    EXPECT_TRUE(message_dispatch_complex.has_subscribers());
    EXPECT_EQ(message_dispatch_complex.public_test_get_subscribers().size(), 1);
    EXPECT_TRUE(message_dispatch_complex.unsubscribe(sub_uuid));
    EXPECT_FALSE(message_dispatch_complex.has_subscribers());
    EXPECT_EQ(message_dispatch_complex.public_test_get_subscribers().size(), 0);

    ComplexData complex_two;
    complex_two.uuid = "886313e1-3b8a-5372-9b90-0c9aee199e5d";
    complex_two.simple_vector.push_back(test_data_three);
    complex_two.mapping[ComplexData::EnumType::VAL_TWO] = "orange";
    message_dispatch_complex.publish_message(complex_two);

    EXPECT_EQ(call_count, 1);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
