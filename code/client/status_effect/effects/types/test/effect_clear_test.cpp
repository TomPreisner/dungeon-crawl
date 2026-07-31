#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include "code/client/status_effect/effects/types/effect_clear.h"

// This Test class is just being used to expose some functions to the 
//  public for use in the Test functions. These are not meant to be available
//  in the normal api, so they are being wrapped in this class instead.
class Effect_Clear_Test : public Status::Effect_Clear {
public:
    explicit Effect_Clear_Test(const YAML::Node& node) : Effect_Clear(node) {}

    const bool public_test_has_valid_amount() const { return test_has_valid_amount(); }
    const float public_test_get_amount() const { return test_get_amount(); }
    const bool public_test_has_valid_callback() const { return test_has_valid_callback(); }
};

std::string k_yaml_file = "code\\client\\status_effect\\effects\\types\\test\\data\\test_clear_data.yaml";

TEST(Effect_Clear, Test_EffectCreation) {
    // A simple load of a yaml file and validate the contents
    YAML::Node config = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(config.IsMap());

    YAML::Node effect_node = config["TestData_Create"];
    EXPECT_TRUE(effect_node);
    EXPECT_TRUE(effect_node.IsSequence());
    
    EXPECT_EQ(5, effect_node.size());

    // NONE
    Effect_Clear_Test test = Effect_Clear_Test(effect_node[0]);
    EXPECT_EQ(Status::EffectType::NONE, test.get_effect_type());
    EXPECT_FALSE(test.is_valid());
    EXPECT_FALSE(test.public_test_has_valid_amount());
    EXPECT_FALSE(test.public_test_has_valid_callback());

    // FOO_BAR
    test = Effect_Clear_Test(effect_node[1]);
    EXPECT_EQ(Status::EffectType::NONE, test.get_effect_type());
    EXPECT_FALSE(test.is_valid());
    EXPECT_FALSE(test.public_test_has_valid_amount());
    EXPECT_FALSE(test.public_test_has_valid_callback());

    // foo_BAR
    test = Effect_Clear_Test(effect_node[2]);
    EXPECT_EQ(Status::EffectType::NONE, test.get_effect_type());
    EXPECT_FALSE(test.is_valid());
    EXPECT_FALSE(test.public_test_has_valid_amount());
    EXPECT_FALSE(test.public_test_has_valid_callback());

    // CLEAR
    test = Effect_Clear_Test(effect_node[3]);
    EXPECT_EQ(Status::EffectType::CLEAR, test.get_effect_type());
    EXPECT_TRUE(test.is_valid());
    EXPECT_TRUE(test.public_test_has_valid_amount());
    EXPECT_EQ(2.f, test.public_test_get_amount());
    EXPECT_FALSE(test.public_test_has_valid_callback());

    // cLeAr
    test = Effect_Clear_Test(effect_node[4]);
    EXPECT_EQ(Status::EffectType::CLEAR, test.get_effect_type());
    EXPECT_TRUE(test.is_valid());
    EXPECT_FALSE(test.public_test_has_valid_amount());
    EXPECT_FALSE(test.public_test_has_valid_callback());
}

TEST(Effect_Clear, Test_EffectProcess) {
    // A simple load of a yaml file and validate the contents
    YAML::Node config = YAML::LoadFile(k_yaml_file);
    Status::EffectData effect_test_value(5.f);

    EXPECT_TRUE(config.IsMap());

    YAML::Node effect_node = config["TestData_Process"];
    EXPECT_TRUE(effect_node);
    EXPECT_TRUE(effect_node.IsSequence());
    
    EXPECT_EQ(3, effect_node.size());
    
    int timesCalled = 0;
    // NONE
    Effect_Clear_Test test = Effect_Clear_Test(effect_node[0]);
    test.RegisterCallback([&](float value, int32_t flags) { 
        ++timesCalled;
        EXPECT_FLOAT_EQ(0.f, value);
    });
    EXPECT_EQ(Status::EffectType::NONE, test.get_effect_type());
    EXPECT_FALSE(test.is_valid());
    EXPECT_FALSE(test.public_test_has_valid_amount());
    EXPECT_TRUE(test.public_test_has_valid_callback());
    EXPECT_FLOAT_EQ(0.f, test.process_effect(effect_test_value));
    EXPECT_EQ(0, timesCalled);
    timesCalled = 0;

    // CLEAR
    test = Effect_Clear_Test(effect_node[1]);
    test.RegisterCallback([&](float value, int32_t flags) { 
        ++timesCalled;
        EXPECT_FLOAT_EQ(0.f, value);
    });
    EXPECT_EQ(Status::EffectType::CLEAR, test.get_effect_type());
    EXPECT_TRUE(test.is_valid());
    EXPECT_TRUE(test.public_test_has_valid_amount());
    EXPECT_TRUE(test.public_test_has_valid_callback());
    EXPECT_FLOAT_EQ(0.f, test.process_effect(effect_test_value));
    EXPECT_EQ(1, timesCalled);
    timesCalled = 0;

    // CLEAR -- no amount listed
    test = Effect_Clear_Test(effect_node[2]);
    test.RegisterCallback([&](float value, int32_t flags) { 
        ++timesCalled;
        EXPECT_FLOAT_EQ(0.f, value);
    });
    EXPECT_EQ(Status::EffectType::CLEAR, test.get_effect_type());
    EXPECT_TRUE(test.is_valid());
    EXPECT_FALSE(test.public_test_has_valid_amount());
    EXPECT_TRUE(test.public_test_has_valid_callback());
    EXPECT_FLOAT_EQ(0.f, test.process_effect(effect_test_value));
    EXPECT_EQ(1, timesCalled);
    timesCalled = 0;
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
