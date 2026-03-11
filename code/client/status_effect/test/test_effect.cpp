#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include "code/client/status_effect/effect.h"
#include "code/core/log_manager.h"

// This Test class is just being used to expose some functions to the 
//  public for use in the Test functions. These are not meant to be available
//  in the normal api, so they are being wrapped in this class instead.
class EffectType_Test : public Status::Effect {
public:
    explicit EffectType_Test(const YAML::Node& node) : Effect(node) {}

    const bool public_test_has_valid_amount() const { return test_has_valid_amount(); }
    const float public_test_get_amount() const { return test_get_amount(); }
    const bool public_test_has_valid_callback() const { return test_has_valid_callback(); }
};

std::string k_yaml_file = "code\\client\\status_effect\\test\\data\\test_effect_data.yaml";

TEST(EffectType, Test_EffectCreation) {
    // A simple load of a yaml file and validate the contents
    YAML::Node config = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(config.IsMap());

    YAML::Node effect_node = config["TestData_Create"];
    EXPECT_TRUE(effect_node);
    EXPECT_TRUE(effect_node.IsSequence());
    
    EXPECT_EQ(14, effect_node.size());

    // NONE
    EffectType_Test test = EffectType_Test(effect_node[0]);
    EXPECT_EQ(Status::EffectType::NONE, test.get_effect_type());
    EXPECT_TRUE(test.public_test_has_valid_amount());
    EXPECT_EQ(2.f, test.public_test_get_amount());
    EXPECT_FALSE(test.public_test_has_valid_callback());

    // FOO_BAR
    test = EffectType_Test(effect_node[1]);
    EXPECT_EQ(Status::EffectType::NONE, test.get_effect_type());
    EXPECT_TRUE(test.public_test_has_valid_amount());
    EXPECT_EQ(2.f, test.public_test_get_amount());
    EXPECT_FALSE(test.public_test_has_valid_callback());

    // foo_BAR
    test = EffectType_Test(effect_node[2]);
    EXPECT_EQ(Status::EffectType::NONE, test.get_effect_type());
    EXPECT_FALSE(test.public_test_has_valid_amount());
    EXPECT_FALSE(test.public_test_has_valid_callback());

    // CLEAR
    test = EffectType_Test(effect_node[3]);
    EXPECT_EQ(Status::EffectType::CLEAR, test.get_effect_type());
    EXPECT_TRUE(test.public_test_has_valid_amount());
    EXPECT_EQ(2.f, test.public_test_get_amount());
    EXPECT_FALSE(test.public_test_has_valid_callback());

    // cLeAr
    test = EffectType_Test(effect_node[4]);
    EXPECT_EQ(Status::EffectType::CLEAR, test.get_effect_type());
    EXPECT_FALSE(test.public_test_has_valid_amount());
    EXPECT_FALSE(test.public_test_has_valid_callback());

    // DAMAGE
    test = EffectType_Test(effect_node[5]);
    EXPECT_EQ(Status::EffectType::DAMAGE, test.get_effect_type());
    EXPECT_TRUE(test.public_test_has_valid_amount());
    EXPECT_EQ(4.1f, test.public_test_get_amount());
    EXPECT_FALSE(test.public_test_has_valid_callback());

    // DAMAGE_PERCENT
    test = EffectType_Test(effect_node[6]);
    EXPECT_EQ(Status::EffectType::DAMAGE_PERCENT, test.get_effect_type());
    EXPECT_TRUE(test.public_test_has_valid_amount());
    EXPECT_EQ(0.3f, test.public_test_get_amount());
    EXPECT_FALSE(test.public_test_has_valid_callback());

    // HEAL
    test = EffectType_Test(effect_node[7]);
    EXPECT_EQ(Status::EffectType::HEAL, test.get_effect_type());
    EXPECT_TRUE(test.public_test_has_valid_amount());
    EXPECT_EQ(2.1f, test.public_test_get_amount());
    EXPECT_FALSE(test.public_test_has_valid_callback());

    // HEAL_PERCENT
    test = EffectType_Test(effect_node[8]);
    EXPECT_EQ(Status::EffectType::HEAL_PERCENT, test.get_effect_type());
    EXPECT_TRUE(test.public_test_has_valid_amount());
    EXPECT_EQ(1.2f, test.public_test_get_amount());
    EXPECT_FALSE(test.public_test_has_valid_callback());

    // MULTIPLY
    test = EffectType_Test(effect_node[9]);
    EXPECT_EQ(Status::EffectType::MULTIPLY, test.get_effect_type());
    EXPECT_TRUE(test.public_test_has_valid_amount());
    EXPECT_EQ(2.f, test.public_test_get_amount());
    EXPECT_FALSE(test.public_test_has_valid_callback());

    // multiply
    test = EffectType_Test(effect_node[10]);
    EXPECT_EQ(Status::EffectType::MULTIPLY, test.get_effect_type());
    EXPECT_TRUE(test.public_test_has_valid_amount());
    EXPECT_EQ(3.f, test.public_test_get_amount());
    EXPECT_FALSE(test.public_test_has_valid_callback());

    // MULTIPLY
    test = EffectType_Test(effect_node[11]);
    EXPECT_EQ(Status::EffectType::MULTIPLY, test.get_effect_type());
    EXPECT_FALSE(test.public_test_has_valid_amount());
    EXPECT_FALSE(test.public_test_has_valid_callback());

    // MULTIPLY
    test = EffectType_Test(effect_node[12]);
    EXPECT_EQ(Status::EffectType::MULTIPLY, test.get_effect_type());
    EXPECT_TRUE(test.public_test_has_valid_amount());
    EXPECT_EQ(5.f, test.public_test_get_amount());
    EXPECT_FALSE(test.public_test_has_valid_callback());

    // MULTIPLY
    test = EffectType_Test(effect_node[13]);
    EXPECT_EQ(Status::EffectType::MULTIPLY, test.get_effect_type());
    EXPECT_FALSE(test.public_test_has_valid_amount());
    EXPECT_FALSE(test.public_test_has_valid_callback());
}

TEST(EffectType, Test_EffectProcess) {
    // A simple load of a yaml file and validate the contents
    YAML::Node config = YAML::LoadFile(k_yaml_file);
    const float effect_test_value = 5.f;

    EXPECT_TRUE(config.IsMap());

    YAML::Node effect_node = config["TestData_Process"];
    EXPECT_TRUE(effect_node);
    EXPECT_TRUE(effect_node.IsSequence());
    
    EXPECT_EQ(7, effect_node.size());
    
    int timesCalled = 0;
    // NONE
    EffectType_Test test = EffectType_Test(effect_node[0]);
    test.RegisterCallback([&](float value) { 
        ++timesCalled;
        EXPECT_FLOAT_EQ(0.f, value);
    });
    EXPECT_EQ(Status::EffectType::NONE, test.get_effect_type());
    EXPECT_TRUE(test.public_test_has_valid_amount());
    EXPECT_TRUE(test.public_test_has_valid_callback());
    EXPECT_FLOAT_EQ(0.f, test.process_effect(effect_test_value));
    EXPECT_EQ(0, timesCalled);
    timesCalled = 0;

    // CLEAR
    test = EffectType_Test(effect_node[1]);
    test.RegisterCallback([&](float value) { 
        ++timesCalled;
        EXPECT_FLOAT_EQ(0.f, value);
    });
    EXPECT_EQ(Status::EffectType::CLEAR, test.get_effect_type());
    EXPECT_TRUE(test.public_test_has_valid_amount());
    EXPECT_TRUE(test.public_test_has_valid_callback());
    EXPECT_FLOAT_EQ(0.f, test.process_effect(effect_test_value));
    EXPECT_EQ(1, timesCalled);
    timesCalled = 0;

    // DAMAGE
    test = EffectType_Test(effect_node[2]);
    test.RegisterCallback([&](float value) { 
        ++timesCalled;
        EXPECT_FLOAT_EQ(4.1f, value);
    });
    EXPECT_EQ(Status::EffectType::DAMAGE, test.get_effect_type());
    EXPECT_TRUE(test.public_test_has_valid_amount());
    EXPECT_FLOAT_EQ(4.1f, test.public_test_get_amount());
    EXPECT_TRUE(test.public_test_has_valid_callback());
    EXPECT_FLOAT_EQ(4.1f, test.process_effect(effect_test_value));
    EXPECT_EQ(1, timesCalled);
    timesCalled = 0;

    // DAMAGE_PERCENT
    test = EffectType_Test(effect_node[3]);
    test.RegisterCallback([&](float value) { 
        ++timesCalled;
        EXPECT_FLOAT_EQ(effect_test_value * 0.3f, value);
    });
    EXPECT_EQ(Status::EffectType::DAMAGE_PERCENT, test.get_effect_type());
    EXPECT_TRUE(test.public_test_has_valid_amount());
    EXPECT_FLOAT_EQ(0.3f, test.public_test_get_amount());
    EXPECT_TRUE(test.public_test_has_valid_callback());
    EXPECT_FLOAT_EQ(effect_test_value * test.public_test_get_amount(), test.process_effect(effect_test_value));
    EXPECT_EQ(1, timesCalled);
    timesCalled = 0;

    // HEAL
    test = EffectType_Test(effect_node[4]);
    test.RegisterCallback([&](float value) { 
        ++timesCalled;
        EXPECT_FLOAT_EQ(2.1f, value);
    });
    EXPECT_EQ(Status::EffectType::HEAL, test.get_effect_type());
    EXPECT_TRUE(test.public_test_has_valid_amount());
    EXPECT_FLOAT_EQ(2.1f, test.public_test_get_amount());
    EXPECT_TRUE(test.public_test_has_valid_callback());
    EXPECT_FLOAT_EQ(2.1f, test.process_effect(effect_test_value));
    EXPECT_EQ(1, timesCalled);
    timesCalled = 0;

    // HEAL_PERCENT
    test = EffectType_Test(effect_node[5]);
    test.RegisterCallback([&](float value) { 
        ++timesCalled;
        EXPECT_FLOAT_EQ(effect_test_value * 1.2f, value);
    });
    EXPECT_EQ(Status::EffectType::HEAL_PERCENT, test.get_effect_type());
    EXPECT_TRUE(test.public_test_has_valid_amount());
    EXPECT_FLOAT_EQ(1.2f, test.public_test_get_amount());
    EXPECT_TRUE(test.public_test_has_valid_callback());
    EXPECT_FLOAT_EQ(effect_test_value * test.public_test_get_amount(), test.process_effect(effect_test_value));
    EXPECT_EQ(1, timesCalled);
    timesCalled = 0;

    // MULTIPLY
    test = EffectType_Test(effect_node[6]);
    test.RegisterCallback([&](float value) { 
        ++timesCalled;
        EXPECT_FLOAT_EQ(effect_test_value * 3.f, value);
    });
    EXPECT_EQ(Status::EffectType::MULTIPLY, test.get_effect_type());
    EXPECT_TRUE(test.public_test_has_valid_amount());
    EXPECT_FLOAT_EQ(3.f, test.public_test_get_amount());
    EXPECT_TRUE(test.public_test_has_valid_callback());
    EXPECT_FLOAT_EQ(effect_test_value * test.public_test_get_amount(), test.process_effect(effect_test_value));
    EXPECT_EQ(1, timesCalled);
    timesCalled = 0;
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
