#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include "code/client/status_effect/status_effect.h"

// This Test class is just being used to expose some functions to the 
//  public for use in the Test functions. These are not meant to be available
//  in the normal api, so they are being wrapped in this class instead.
class StatusEffect_Test : public Status::StatusEffect {
public:
    explicit StatusEffect_Test(const YAML::Node& node) : StatusEffect(node) {}
    
    const bool public_test_has_valid_update_rate_turns() const { return test_has_valid_update_rate_turns(); }
    const int public_test_get_update_rate_turns() const { return test_get_update_rate_turns(); }
    const bool public_test_has_valid_duration_turns() const { return test_has_valid_duration_turns(); }
    const int public_test_get_duration_turns() const { return test_get_duration_turns(); }
    const int public_test_get_current_tick() const { return test_get_current_tick(); }
    const std::vector<Status::Effect>& public_test_get_heal_effects() const { return test_get_heal_effects(); }
    const std::vector<Status::Effect>& public_test_get_damage_effects() const { return test_get_damage_effects(); }
    const std::vector<Status::Effect>& public_test_get_update_effects() const { return test_get_update_effects(); }

    virtual void clear_callback() override {
        ++m_clear_callback_count;
        StatusEffect::clear_callback();
    }
    virtual void heal_callback(float amount) override {
        ++m_heal_callback_count;
        StatusEffect::heal_callback(amount);
    }
    virtual void damage_callback(float amount) override {
        ++m_damage_callback_count;
        StatusEffect::damage_callback(amount);
    }
    virtual void augment_callback(float amount) override {
        ++m_augment_callback_count;
        StatusEffect::augment_callback(amount);
    }

    int m_clear_callback_count = 0;
    int m_heal_callback_count = 0;
    int m_damage_callback_count = 0;
    int m_augment_callback_count = 0;
};

std::string k_yaml_file = "code\\client\\status_effect\\test\\data\\test_status_effect_data.yaml";

TEST(StatusEffect, Test_StatusEffectCreation) {
    // A simple load of a yaml file and validate the contents
    YAML::Node config = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(config.IsMap());

    YAML::Node golden_case_node = config["GoldenCase"];
    EXPECT_TRUE(golden_case_node);
    EXPECT_TRUE(golden_case_node.IsMap());
    StatusEffect_Test golden_case(golden_case_node);
    EXPECT_EQ(golden_case.get_type(), Status::StatusEffectType::FIRE);
    EXPECT_EQ(golden_case.get_icon(), "flame_icon_small.png");
    EXPECT_EQ(golden_case.get_vfx(), "fire_effect.vfx");
    EXPECT_EQ(golden_case.get_sfx(), "fire_crackle.wav");
    EXPECT_TRUE(golden_case.public_test_has_valid_update_rate_turns());
    EXPECT_EQ(golden_case.public_test_get_update_rate_turns(), 1);
    EXPECT_TRUE(golden_case.public_test_has_valid_update_rate_turns());
    EXPECT_EQ(golden_case.public_test_get_duration_turns(), 10);
    EXPECT_EQ(golden_case.public_test_get_current_tick(), -1);
    EXPECT_EQ(golden_case.public_test_get_heal_effects().size(), 1);
    EXPECT_EQ(golden_case.public_test_get_damage_effects().size(), 1);
    EXPECT_EQ(golden_case.public_test_get_update_effects().size(), 2);

    
    YAML::Node invalid_not_enum_node = config["InvalidType_notEnum"];
    EXPECT_TRUE(invalid_not_enum_node);
    EXPECT_TRUE(invalid_not_enum_node.IsMap());
    StatusEffect_Test invalid_not_enum(invalid_not_enum_node);
    EXPECT_EQ(invalid_not_enum.get_type(), Status::StatusEffectType::NONE);
    EXPECT_EQ(invalid_not_enum.get_icon(), "true"); //< yaml does the implicit type conversion
    EXPECT_EQ(invalid_not_enum.get_vfx(), "15");    //< yaml does the implicit type conversion
    EXPECT_EQ(invalid_not_enum.get_sfx(), "12");    //< yaml does the implicit type conversion
    EXPECT_FALSE(invalid_not_enum.public_test_has_valid_update_rate_turns());
    EXPECT_EQ(invalid_not_enum.public_test_get_update_rate_turns(), -1);
    EXPECT_FALSE(invalid_not_enum.public_test_has_valid_update_rate_turns());
    EXPECT_EQ(invalid_not_enum.public_test_get_duration_turns(), -1);
    EXPECT_EQ(invalid_not_enum.public_test_get_current_tick(), -1);
    EXPECT_EQ(invalid_not_enum.public_test_get_heal_effects().size(), 1);
    EXPECT_TRUE(invalid_not_enum.public_test_get_damage_effects().empty());
    EXPECT_EQ(invalid_not_enum.public_test_get_update_effects().size(), 1);

    
    YAML::Node invalid_not_scalar_node = config["InvalidType_notScalar"];
    EXPECT_TRUE(invalid_not_scalar_node);
    EXPECT_TRUE(invalid_not_scalar_node.IsMap());
    StatusEffect_Test invalid_not_scalar(invalid_not_scalar_node);
    EXPECT_EQ(invalid_not_scalar.get_type(), Status::StatusEffectType::NONE);
    EXPECT_EQ(invalid_not_scalar.get_icon(), "");
    EXPECT_EQ(invalid_not_scalar.get_vfx(), "");
    EXPECT_EQ(invalid_not_scalar.get_sfx(), "");
    EXPECT_FALSE(invalid_not_scalar.public_test_has_valid_update_rate_turns());
    EXPECT_EQ(invalid_not_scalar.public_test_get_update_rate_turns(), -1);
    EXPECT_FALSE(invalid_not_scalar.public_test_has_valid_update_rate_turns());
    EXPECT_EQ(invalid_not_scalar.public_test_get_duration_turns(), -1);
    EXPECT_EQ(invalid_not_scalar.public_test_get_current_tick(), -1);
    EXPECT_EQ(invalid_not_scalar.public_test_get_heal_effects().size(), 4);
    EXPECT_TRUE(invalid_not_scalar.public_test_get_damage_effects().empty());
    EXPECT_TRUE(invalid_not_scalar.public_test_get_update_effects().empty());
}

TEST(StatusEffect, Test_StatusEffectProcessing) {
    // A simple load of a yaml file and validate the contents
    YAML::Node config = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(config.IsMap());

    YAML::Node golden_case_node = config["GoldenCase"];
    EXPECT_TRUE(golden_case_node);
    EXPECT_TRUE(golden_case_node.IsMap());
    StatusEffect_Test golden_case(golden_case_node);
    
    // Assume that the creation tests catch any creation error and this test only
    // checks operation
    EXPECT_EQ(golden_case.m_clear_callback_count, 0);
    EXPECT_EQ(golden_case.m_heal_callback_count, 0);
    EXPECT_EQ(golden_case.m_damage_callback_count, 0);
    EXPECT_EQ(golden_case.m_augment_callback_count, 0);

    EXPECT_NE(golden_case.get_uuid(), "");
    golden_case.on_update();  //< update needs to be pumped once to prime the state machine
    golden_case.activate();
    EXPECT_EQ(golden_case.public_test_get_current_tick(), -1);

    golden_case.on_update();
    EXPECT_EQ(golden_case.public_test_get_current_tick(), 0);

    const float heal_amt = 4.f;
    const float damage_amt = 3.f;
    // intentionally update it more than the duration to force stop and termination
    const int update_amount = golden_case.public_test_get_duration_turns() * 2;
    for (int i = 0; i < update_amount; ++i) {
        golden_case.on_update();
        if (i%3 == 0) {
            golden_case.on_heal(heal_amt);
        }
        if (i%4 == 0) {
            golden_case.on_damage(damage_amt);            
        }
    }

    EXPECT_EQ(golden_case.public_test_get_current_tick(), 11);
    EXPECT_EQ(golden_case.m_clear_callback_count, 0);
    EXPECT_EQ(golden_case.m_heal_callback_count, 4 + 10);
    EXPECT_EQ(golden_case.m_damage_callback_count, 10);
    EXPECT_EQ(golden_case.m_augment_callback_count, 3);    

    // This is similar but since the type is invalid, the callbacks are not processed and the state 
    YAML::Node invalid_not_enum_node = config["InvalidType_notEnum"];
    EXPECT_TRUE(invalid_not_enum_node);
    EXPECT_TRUE(invalid_not_enum_node.IsMap());
    StatusEffect_Test invalid_not_enum(invalid_not_enum_node);
    
    // Assume that the creation tests catch any creation error and this test only
    // checks operation
    // This test data is interesting.  The duration is -1 so it runs forever, and the 
    //  update_rate is -1, so it never calls the on_update effects.  Logically these make 
    //  sense and add for more design options so leaving as intentional.
    EXPECT_EQ(invalid_not_enum.m_clear_callback_count, 0);
    EXPECT_EQ(invalid_not_enum.m_heal_callback_count, 0);
    EXPECT_EQ(invalid_not_enum.m_damage_callback_count, 0);
    EXPECT_EQ(invalid_not_enum.m_augment_callback_count, 0);

    EXPECT_NE(invalid_not_enum.get_uuid(), "");
    invalid_not_enum.on_update(); //< update needs to be pumped once to prime the state machine
    invalid_not_enum.activate();
    EXPECT_EQ(invalid_not_enum.public_test_get_current_tick(), -1);

    invalid_not_enum.on_update();
    EXPECT_EQ(invalid_not_enum.public_test_get_current_tick(), 0);
    
    // intentionally update it more than the duration to force stop and termination
    for (int i = 0; i < update_amount; ++i) {
        invalid_not_enum.on_update();
        if (i > 0 && i%3 == 0) {
            invalid_not_enum.on_heal(heal_amt);
        }
        if (i > 0 && i%4 == 0) {
            invalid_not_enum.on_damage(damage_amt);            
        }
    }

    EXPECT_EQ(invalid_not_enum.public_test_get_current_tick(), 4);
    EXPECT_EQ(invalid_not_enum.m_clear_callback_count, 1);
    EXPECT_EQ(invalid_not_enum.m_heal_callback_count, 0);
    EXPECT_EQ(invalid_not_enum.m_damage_callback_count, 0);
    EXPECT_EQ(invalid_not_enum.m_augment_callback_count, 0);    
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
