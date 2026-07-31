#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include "code/client/status_effect/effects/types/effect_heal_multiply.h"
#include "code/client/messages/proto/heal.pb.h"

// This Test class is just being used to expose some functions to the 
//  public for use in the Test functions. These are not meant to be available
//  in the normal api, so they are being wrapped in this class instead.
class Effect_Heal_Multiply_Test : public Status::Effect_Heal_Multiply {
public:
    explicit Effect_Heal_Multiply_Test(const YAML::Node& node) : Effect_Heal_Multiply(node) {}

    const bool public_test_has_valid_amount() const { return test_has_valid_amount(); }
    const float public_test_get_amount() const { return test_get_amount(); }
    const bool public_test_has_valid_callback() const { return test_has_valid_callback(); }
    const int32_t public_test_get_heal_type() const { return test_get_heal_type(); }
    const bool has_heal_type(code::client::messages::Heal::HealType healType) const {
        return test_has_heal_type() && (healType == (test_get_heal_type() & healType));
    }
};

std::string k_yaml_file = "code\\client\\status_effect\\effects\\types\\test\\data\\test_heal_mult_data.yaml";

TEST(Effect_Heal_Multiply, Test_EffectCreation) {
    // A simple load of a yaml file and validate the contents
    YAML::Node config = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(config.IsMap());

    YAML::Node effect_node = config["TestData_Create"];
    EXPECT_TRUE(effect_node);
    EXPECT_TRUE(effect_node.IsSequence());
    
    EXPECT_EQ(9, effect_node.size());

    // HEAL_MULTIPLY
    Effect_Heal_Multiply_Test test = Effect_Heal_Multiply_Test(effect_node[0]);
    EXPECT_EQ(Status::EffectType::HEAL_MULTIPLY, test.get_effect_type());
    EXPECT_TRUE(test.is_valid());
    EXPECT_TRUE(test.public_test_has_valid_amount());
    EXPECT_EQ(2.1f, test.public_test_get_amount());
    EXPECT_FALSE(test.public_test_has_valid_callback());
    EXPECT_TRUE(test.has_heal_type(code::client::messages::Heal::MAGIC));
    EXPECT_TRUE(test.has_heal_type(code::client::messages::Heal::HOLY));
    EXPECT_EQ(test.public_test_get_heal_type(), code::client::messages::Heal::MAGIC |
                                                     code::client::messages::Heal::HOLY);

    // HEAL_MULTIPLY
    test = Effect_Heal_Multiply_Test(effect_node[1]);
    EXPECT_EQ(Status::EffectType::NONE, test.get_effect_type());
    EXPECT_FALSE(test.is_valid());
    EXPECT_FALSE(test.public_test_has_valid_amount());
    EXPECT_FALSE(test.public_test_has_valid_callback());
    EXPECT_FALSE(test.has_heal_type(code::client::messages::Heal::POTION));
    EXPECT_FALSE(test.has_heal_type(code::client::messages::Heal::MAGIC));
    EXPECT_FALSE(test.has_heal_type(code::client::messages::Heal::HOLY));

    // HEAL_MULTIPLY
    test = Effect_Heal_Multiply_Test(effect_node[2]);
    EXPECT_EQ(Status::EffectType::NONE, test.get_effect_type());
    EXPECT_FALSE(test.is_valid());
    EXPECT_FALSE(test.public_test_has_valid_amount());
    EXPECT_FALSE(test.public_test_has_valid_callback());
    EXPECT_FALSE(test.has_heal_type(code::client::messages::Heal::POTION));
    EXPECT_FALSE(test.has_heal_type(code::client::messages::Heal::MAGIC));
    EXPECT_FALSE(test.has_heal_type(code::client::messages::Heal::HOLY));

    // HEAL_MULTIPLY
    test = Effect_Heal_Multiply_Test(effect_node[3]);
    EXPECT_EQ(Status::EffectType::NONE, test.get_effect_type());
    EXPECT_FALSE(test.is_valid());
    EXPECT_FALSE(test.public_test_has_valid_amount());
    EXPECT_FALSE(test.public_test_has_valid_callback());
    EXPECT_FALSE(test.has_heal_type(code::client::messages::Heal::POTION));
    EXPECT_FALSE(test.has_heal_type(code::client::messages::Heal::MAGIC));
    EXPECT_FALSE(test.has_heal_type(code::client::messages::Heal::HOLY));

    // HEAL_MULTIPLY
    test = Effect_Heal_Multiply_Test(effect_node[4]);
    EXPECT_EQ(Status::EffectType::HEAL_MULTIPLY, test.get_effect_type());
    EXPECT_TRUE(test.is_valid());
    EXPECT_TRUE(test.public_test_has_valid_amount());
    EXPECT_EQ(1.2f, test.public_test_get_amount());
    EXPECT_FALSE(test.public_test_has_valid_callback());
    EXPECT_TRUE(test.has_heal_type(code::client::messages::Heal::POTION));
    EXPECT_EQ(test.public_test_get_heal_type(), code::client::messages::Heal::POTION);

    // heal_multiply
    test = Effect_Heal_Multiply_Test(effect_node[5]);
    EXPECT_EQ(Status::EffectType::HEAL_MULTIPLY, test.get_effect_type());
    EXPECT_TRUE(test.is_valid());
    EXPECT_TRUE(test.public_test_has_valid_amount());
    EXPECT_EQ(3.0f, test.public_test_get_amount());
    EXPECT_FALSE(test.public_test_has_valid_callback());
    EXPECT_TRUE(test.has_heal_type(code::client::messages::Heal::POTION));
    EXPECT_EQ(test.public_test_get_heal_type(), code::client::messages::Heal::POTION);

    // HEAL_MULTIPLY
    test = Effect_Heal_Multiply_Test(effect_node[6]);
    EXPECT_EQ(Status::EffectType::NONE, test.get_effect_type());
    EXPECT_FALSE(test.is_valid());
    EXPECT_FALSE(test.public_test_has_valid_amount());
    EXPECT_FALSE(test.public_test_has_valid_callback());
    EXPECT_FALSE(test.has_heal_type(code::client::messages::Heal::POTION));
    EXPECT_FALSE(test.has_heal_type(code::client::messages::Heal::MAGIC));
    EXPECT_FALSE(test.has_heal_type(code::client::messages::Heal::HOLY));

    // HEAL_MULTIPLY
    test = Effect_Heal_Multiply_Test(effect_node[7]);
    EXPECT_EQ(Status::EffectType::HEAL_MULTIPLY, test.get_effect_type());
    EXPECT_TRUE(test.is_valid());
    EXPECT_TRUE(test.public_test_has_valid_amount());
    EXPECT_EQ(5.0f, test.public_test_get_amount());
    EXPECT_FALSE(test.public_test_has_valid_callback());
    EXPECT_TRUE(test.has_heal_type(code::client::messages::Heal::POTION));
    EXPECT_EQ(test.public_test_get_heal_type(), code::client::messages::Heal::POTION);

    // HEAL_MULTIPLY
    test = Effect_Heal_Multiply_Test(effect_node[8]);
    EXPECT_EQ(Status::EffectType::NONE, test.get_effect_type());
    EXPECT_FALSE(test.is_valid());
    EXPECT_FALSE(test.public_test_has_valid_amount());
    EXPECT_FALSE(test.public_test_has_valid_callback());
    EXPECT_FALSE(test.has_heal_type(code::client::messages::Heal::POTION));
    EXPECT_FALSE(test.has_heal_type(code::client::messages::Heal::MAGIC));
    EXPECT_FALSE(test.has_heal_type(code::client::messages::Heal::HOLY));
}

TEST(Effect_Heal_Multiply, Test_EffectProcess) {
    // A simple load of a yaml file and validate the contents
    YAML::Node config = YAML::LoadFile(k_yaml_file);
    const float effect_test_value = 5.f;
    // yes, these are getting stored in a base version. that function is public, the sepcialization is private.
    std::shared_ptr<Status::EffectData> effect_test = 
        std::make_shared<Status::EffectData_Heal_Type>(effect_test_value, (int32_t)code::client::messages::Heal::POTION);
    std::shared_ptr<Status::EffectData> effect_test_double = 
        std::make_shared<Status::EffectData_Heal_Type>(effect_test_value, (int32_t)(code::client::messages::Heal::MAGIC |
                                                                          code::client::messages::Heal::HOLY));
    std::shared_ptr<Status::EffectData> effect_test_invalid = 
        std::make_shared<Status::EffectData_Heal_Type>(effect_test_value, (int32_t)code::client::messages::Heal::MAGIC);

    EXPECT_TRUE(config.IsMap());

    YAML::Node effect_node = config["TestData_Process"];
    EXPECT_TRUE(effect_node);
    EXPECT_TRUE(effect_node.IsSequence());
    
    EXPECT_EQ(2, effect_node.size());
    
    int timesCalled = 0;
    // HEAL
    Effect_Heal_Multiply_Test test = Effect_Heal_Multiply_Test(effect_node[0]);
    test.RegisterCallback([&](float value, int32_t flags) { 
        ++timesCalled;
        EXPECT_FLOAT_EQ(effect_test_value * 2.1f, value);
    });
    EXPECT_EQ(Status::EffectType::HEAL_MULTIPLY, test.get_effect_type());
    EXPECT_TRUE(test.public_test_has_valid_amount());
    EXPECT_FLOAT_EQ(2.1f, test.public_test_get_amount());
    EXPECT_TRUE(test.public_test_has_valid_callback());
    EXPECT_FLOAT_EQ(effect_test_value * test.public_test_get_amount(), test.process_effect(*(effect_test_double.get())));
    EXPECT_EQ(1, timesCalled);
    EXPECT_FLOAT_EQ(0.0f, test.process_effect(*(effect_test.get())));
    EXPECT_EQ(1, timesCalled);
    timesCalled = 0;

    // HEAL_MULTIPLY
    test = Effect_Heal_Multiply_Test(effect_node[1]);
    test.RegisterCallback([&](float value, int32_t flags) { 
        ++timesCalled;
        EXPECT_FLOAT_EQ(effect_test_value * 1.2f, value);
    });
    EXPECT_EQ(Status::EffectType::HEAL_MULTIPLY, test.get_effect_type());
    EXPECT_TRUE(test.public_test_has_valid_amount());
    EXPECT_FLOAT_EQ(1.2f, test.public_test_get_amount());
    EXPECT_TRUE(test.public_test_has_valid_callback());
    EXPECT_FLOAT_EQ(effect_test_value * test.public_test_get_amount(), test.process_effect(*(effect_test.get())));
    EXPECT_EQ(1, timesCalled);
    EXPECT_FLOAT_EQ(0.0f, test.process_effect(*(effect_test_invalid.get())));
    EXPECT_EQ(1, timesCalled);
    timesCalled = 0;
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
