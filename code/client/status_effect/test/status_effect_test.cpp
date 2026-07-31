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
    
    const bool public_test_has_valid_update_rate_ms() const { return test_has_valid_update_rate_ms(); }
    const std::chrono::milliseconds public_test_get_update_rate_ms() const { return test_get_update_rate_ms(); }
    const bool public_test_has_valid_duration_ms() const { return test_has_valid_duration_ms(); }
    const std::chrono::milliseconds public_test_get_duration_ms() const { return test_get_duration_ms(); }
    const std::chrono::milliseconds public_test_get_current_tick_ms() const { return test_get_current_tick_ms(); }
    const Effect_List& public_test_get_heal_effects() const { return test_get_heal_effects(); }
    const Effect_List& public_test_get_damage_effects() const { return test_get_damage_effects(); }
    const Effect_List& public_test_get_update_effects() const { return test_get_update_effects(); }

    virtual void clear_callback() override {
        ++m_clear_callback_count;
        StatusEffect::clear_callback();
    }
    // There is nothing to callback into for the context of these tests, so eat the call
    virtual void heal_callback(float amount, const int32_t heal_type) override {
        ++m_heal_callback_count;
    }
    virtual void damage_callback(float amount, const int32_t damage_flags) override {
        ++m_damage_callback_count;
    }
    virtual void augment_callback(float amount) override {
        ++m_augment_callback_count;
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
    EXPECT_TRUE(golden_case.public_test_has_valid_update_rate_ms());
    EXPECT_EQ(golden_case.public_test_get_update_rate_ms().count(), 1000);
    EXPECT_TRUE(golden_case.public_test_has_valid_duration_ms());
    EXPECT_EQ(golden_case.public_test_get_duration_ms().count(), 10200);
    EXPECT_EQ(golden_case.public_test_get_current_tick_ms().count(), -1);
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
    EXPECT_FALSE(invalid_not_enum.public_test_has_valid_update_rate_ms());
    EXPECT_EQ(invalid_not_enum.public_test_get_update_rate_ms().count(), -1);
    EXPECT_FALSE(invalid_not_enum.public_test_has_valid_duration_ms());
    EXPECT_EQ(invalid_not_enum.public_test_get_duration_ms().count(), -1);
    EXPECT_EQ(invalid_not_enum.public_test_get_current_tick_ms().count(), -1);
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
    EXPECT_FALSE(invalid_not_scalar.public_test_has_valid_update_rate_ms());
    EXPECT_EQ(invalid_not_scalar.public_test_get_update_rate_ms().count(), -1);
    EXPECT_FALSE(invalid_not_scalar.public_test_has_valid_duration_ms());
    EXPECT_EQ(invalid_not_scalar.public_test_get_duration_ms().count(), -1);
    EXPECT_EQ(invalid_not_scalar.public_test_get_current_tick_ms().count(), -1);
    EXPECT_EQ(invalid_not_scalar.public_test_get_heal_effects().size(), 4);
    EXPECT_TRUE(invalid_not_scalar.public_test_get_damage_effects().empty());
    EXPECT_TRUE(invalid_not_scalar.public_test_get_update_effects().empty());
}

///////////////////////////////////////////////////////////////////////////////////////////////
struct ExpectedResult {
    int tick_count = 0;
    int clear_call_count = 0;
    int heal_call_count = 0;
    int damage_call_count = 0;
};

void StatusEffectProcessingTest(StatusEffect_Test& statusEffect, const int update_amount, ExpectedResult result) {
    const std::chrono::milliseconds update_rate(500); // 0.5 seconds

    // Assume that the creation tests catch any creation error and this test only
    // checks operation
    EXPECT_EQ(statusEffect.m_clear_callback_count, 0);
    EXPECT_EQ(statusEffect.m_heal_callback_count, 0);
    EXPECT_EQ(statusEffect.m_damage_callback_count, 0);

    EXPECT_NE(statusEffect.get_uuid(), "");
    statusEffect.on_update(update_rate);  //< update needs to be pumped once to prime the state machine
    statusEffect.activate();
    EXPECT_EQ(statusEffect.public_test_get_current_tick_ms().count(), -1);

    statusEffect.on_update(update_rate);
    EXPECT_EQ(statusEffect.public_test_get_current_tick_ms().count(), 0);

    const float heal_amt = 4.f;
    const float damage_amt = 3.f;
    for (int i = 0; i < update_amount; ++i) {
        statusEffect.on_update(update_rate);
        if (i > 0 && i%3 == 0) {
            code::client::messages::Heal test_heal;
            test_heal.set_heal_type(code::client::messages::Heal::POTION);
            test_heal.set_amount(heal_amt);
            statusEffect.on_heal(test_heal);
        }
        if (i > 0 && i%4 == 0) {
            code::client::messages::Damage test_damage;
            test_damage.set_damage_type(code::client::messages::Damage::POISON);
            test_damage.set_amount(damage_amt);
            statusEffect.on_damage(test_damage);
        }
    }

    EXPECT_EQ(statusEffect.public_test_get_current_tick_ms().count(), result.tick_count);
    EXPECT_EQ(statusEffect.m_clear_callback_count, result.clear_call_count);
    EXPECT_EQ(statusEffect.m_heal_callback_count,result.heal_call_count);
    EXPECT_EQ(statusEffect.m_damage_callback_count, result.damage_call_count);
}

TEST(StatusEffect, Test_StatusEffectProcessing) {
    // A simple load of a yaml file and validate the contents
    YAML::Node config = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(config.IsMap());

    YAML::Node golden_case_node = config["GoldenCase"];
    EXPECT_TRUE(golden_case_node);
    EXPECT_TRUE(golden_case_node.IsMap());
    StatusEffect_Test golden_case(golden_case_node);
 
    // intentionally update it more than the duration to force stop and termination
    StatusEffectProcessingTest(golden_case, 40, {
        10500,  // tick_count
        0,      // clear_call_count
        6 + 10, // heal_call_count
        15,     // damage_call_count
    });

    // This is similar but since the type is invalid, the callbacks are not processed
    // Assume that the creation tests catch any creation error and this test only
    // checks operation
    YAML::Node invalid_not_enum_node = config["InvalidType_notEnum"];
    EXPECT_TRUE(invalid_not_enum_node);
    EXPECT_TRUE(invalid_not_enum_node.IsMap());
    StatusEffect_Test invalid_not_enum(invalid_not_enum_node);

    StatusEffectProcessingTest(invalid_not_enum, 40, {
        2000,   // tick_count
        1,      // clear_call_count
        0,      // heal_call_count
        0,      // damage_call_count
    });
}


TEST(StatusEffect, Test_StatusEffectProcessing_Copy) {
    // A simple load of a yaml file and validate the contents
    YAML::Node config = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(config.IsMap());

    // Create the Status effect using the new operator, create a version with 
    //  the copy constructor, then delete the original and null it out
    YAML::Node golden_case_node = config["GoldenCase"];
    EXPECT_TRUE(golden_case_node);
    EXPECT_TRUE(golden_case_node.IsMap());
    StatusEffect_Test* golden_case = new StatusEffect_Test(golden_case_node);
    StatusEffect_Test golden_case_copy(*golden_case);
    delete(golden_case);
    golden_case = nullptr;
 
    // intentionally update it more than the duration to force stop and termination
    StatusEffectProcessingTest(golden_case_copy, 40, {
        10500,  // tick_count
        0,      // clear_call_count
        6 + 10, // heal_call_count
        15,     // damage_call_count
    });

    // This is similar but since the type is invalid, the callbacks are not processed
    // Assume that the creation tests catch any creation error and this test only
    // checks operation
    YAML::Node invalid_not_enum_node = config["InvalidType_notEnum"];
    EXPECT_TRUE(invalid_not_enum_node);
    EXPECT_TRUE(invalid_not_enum_node.IsMap());
    StatusEffect_Test* invalid_not_enum = new StatusEffect_Test(invalid_not_enum_node);
    StatusEffect_Test invalid_not_enum_copy(*invalid_not_enum);
    delete(invalid_not_enum);
    invalid_not_enum = nullptr;

    StatusEffectProcessingTest(invalid_not_enum_copy, 40, {
        2000,   // tick_count
        1,      // clear_call_count
        0,      // heal_call_count
        0,      // damage_call_count
    });
}
///////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
