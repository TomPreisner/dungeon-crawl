#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include "code/client/damage/proto/damage.pb.h"
#include "code/client/health/proto/heal.pb.h"
#include "code/client/status_effect/status_effect.h"
#include "code/client/status_effect/status_effect_library.h"
#include "code/client/modules/health/status_effect_immune_module.h"

// This Test class is just being used to expose some data to the 
//  public for use in the Test functions. These are not meant to be available
//  in the normal api, so they are being wrapped in this class instead.
class StatusEffectImmune_Module_Test : public Module::StatusEffectImmune_Module {
public:
    explicit StatusEffectImmune_Module_Test(const std::string& name) : StatusEffectImmune_Module(name) {}
    
    std::optional<std::string> test_get_status_effect_name() { return m_status_effect_name; }
    std::optional<Status::StatusEffectType> test_get_status_effect_type() { return m_status_effect_type; }
    std::optional<float> test_get_percent_chance_amount() { return m_percent_chance_amount; }
};

std::string k_yaml_file = "code\\client\\modules\\health\\test\\data\\test_status_effect_immune_module.yaml";
std::string k_yaml_library_file = "code\\client\\modules\\health\\test\\data\\test_status_effect_library_data.yaml";

TEST(StatusEffectImmune_Module, Test_StatusEffectImmuneModule_StatusType) {
    Status::StatusEffectLibrary::get_Instance()->init_from_file(k_yaml_library_file);

    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_statusType = modules["TestData_StatusType"];
    EXPECT_TRUE(test_statusType);
    EXPECT_TRUE(test_statusType.IsMap());
    StatusEffectImmune_Module_Test test_case_statusType("TestData_StatusType");
    EXPECT_EQ(test_case_statusType.get_module_name(), "TestData_StatusType");
    EXPECT_TRUE(test_case_statusType.init_module(test_statusType));
    EXPECT_FALSE(test_case_statusType.test_get_status_effect_name().has_value());
    EXPECT_TRUE(test_case_statusType.test_get_status_effect_type().has_value());
    EXPECT_EQ(test_case_statusType.test_get_status_effect_type().value(), Status::StatusEffectType::FIRE);
    EXPECT_FALSE(test_case_statusType.test_get_percent_chance_amount().has_value());

    Damage test_output;
    std::string* value = test_output.add_status_effect();
    ASSERT_TRUE(value != nullptr);
    *value = "Ignite_V1";
    test_case_statusType.process_damage(test_output);
    EXPECT_EQ(test_output.damage_type(), Damage::NONE);
    EXPECT_FLOAT_EQ(test_output.amount(), 0.0f);
    EXPECT_EQ(test_output.status_effect_size(), 1);
    EXPECT_EQ(test_output.status_effect(0), "");

    Damage test_output_two;
    test_output_two.set_damage_type(Damage::FIRE);
    test_output_two.set_amount(3.2f);
    value = test_output_two.add_status_effect();
    ASSERT_TRUE(value != nullptr);
    *value = "Venom_V1";
    test_case_statusType.process_damage(test_output_two);
    EXPECT_EQ(test_output_two.damage_type() & Damage::FIRE, Damage::FIRE);
    EXPECT_FLOAT_EQ(test_output_two.amount(), 3.2f);
    EXPECT_EQ(test_output_two.status_effect_size(), 1);
    EXPECT_EQ(test_output_two.status_effect(0), "Venom_V1");
    
    Damage test_output_three;
    test_output_three.set_damage_type(Damage::FIRE);
    test_output_three.set_amount(3.2f);
    value = test_output_three.add_status_effect();
    ASSERT_TRUE(value != nullptr);
    *value = "Ignite_V1";
    value = test_output_three.add_status_effect();
    ASSERT_TRUE(value != nullptr);
    *value = "Foobar_V7";
    value = test_output_three.add_status_effect();
    ASSERT_TRUE(value != nullptr);
    *value = "Venom_V1";
    test_case_statusType.process_damage(test_output_three);
    EXPECT_EQ(test_output_three.damage_type() & Damage::FIRE, Damage::FIRE);
    EXPECT_FLOAT_EQ(test_output_three.amount(), 3.2f);
    EXPECT_EQ(test_output_three.status_effect_size(), 3);
    EXPECT_EQ(test_output_three.status_effect(0), "");
    EXPECT_EQ(test_output_three.status_effect(1), "Foobar_V7");
    EXPECT_EQ(test_output_three.status_effect(2), "Venom_V1");
    
    // Healing should not be affected
    Heal test_heal;
    test_heal.set_type(Heal::POTION);
    test_heal.set_amount(3.2f);
    test_case_statusType.process_heal(test_heal);
    EXPECT_EQ(test_heal.type(), Heal::POTION);
    EXPECT_FLOAT_EQ(test_heal.amount(), 3.2f);

    Status::StatusEffectLibrary::get_Instance()->shutdown();
}

TEST(StatusEffectImmune_Module, Test_StatusEffectImmuneModule_StatusType_Invalid) {
    Status::StatusEffectLibrary::get_Instance()->init_from_file(k_yaml_library_file);

    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_statusType_Invalid = modules["TestData_StatusType_Invalid"];
    EXPECT_TRUE(test_statusType_Invalid);
    EXPECT_TRUE(test_statusType_Invalid.IsMap());
    StatusEffectImmune_Module_Test test_case_statusType_Invalid("TestData_StatusType_Invalid");
    EXPECT_EQ(test_case_statusType_Invalid.get_module_name(), "TestData_StatusType_Invalid");
    EXPECT_FALSE(test_case_statusType_Invalid.init_module(test_statusType_Invalid));
    EXPECT_FALSE(test_case_statusType_Invalid.test_get_status_effect_name().has_value());
    EXPECT_FALSE(test_case_statusType_Invalid.test_get_status_effect_type().has_value());
    EXPECT_FALSE(test_case_statusType_Invalid.test_get_percent_chance_amount().has_value());

    // Nothing happens
    Damage test_output;
    std::string* value = test_output.add_status_effect();
    ASSERT_TRUE(value != nullptr);
    *value = "Ignite_V1";
    test_case_statusType_Invalid.process_damage(test_output);
    EXPECT_EQ(test_output.damage_type(), Damage::NONE);
    EXPECT_FLOAT_EQ(test_output.amount(), 0.0f);
    EXPECT_EQ(test_output.status_effect_size(), 1);
    EXPECT_EQ(test_output.status_effect(0), "Ignite_V1");

    Damage test_output_two;
    test_output_two.set_damage_type(Damage::FIRE);
    test_output_two.set_amount(3.2f);
    value = test_output_two.add_status_effect();
    ASSERT_TRUE(value != nullptr);
    *value = "Ignite_V1";
    value = test_output_two.add_status_effect();
    ASSERT_TRUE(value != nullptr);
    *value = "Foobar_V7";
    value = test_output_two.add_status_effect();
    ASSERT_TRUE(value != nullptr);
    *value = "Venom_V1";
    test_case_statusType_Invalid.process_damage(test_output_two);
    EXPECT_EQ(test_output_two.damage_type() & Damage::FIRE, Damage::FIRE);
    EXPECT_FLOAT_EQ(test_output_two.amount(), 3.2f);
    EXPECT_EQ(test_output_two.status_effect_size(), 3);
    EXPECT_EQ(test_output_two.status_effect(0), "Ignite_V1");
    EXPECT_EQ(test_output_two.status_effect(1), "Foobar_V7");
    EXPECT_EQ(test_output_two.status_effect(2), "Venom_V1");

    Heal test_heal;
    test_heal.set_type(Heal::POTION);
    test_heal.set_amount(3.2f);
    test_case_statusType_Invalid.process_heal(test_heal);
    EXPECT_EQ(test_heal.type(), Heal::POTION);
    EXPECT_FLOAT_EQ(test_heal.amount(), 3.2f);

    Status::StatusEffectLibrary::get_Instance()->shutdown();
}

TEST(StatusEffectImmune_Module, Test_StatusEffectImmuneModule_StatusType_50_Percent) {
    Status::StatusEffectLibrary::get_Instance()->init_from_file(k_yaml_library_file);

    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_statusType_50 = modules["TestData_StatusType_50_Percent"];
    EXPECT_TRUE(test_statusType_50);
    EXPECT_TRUE(test_statusType_50.IsMap());
    StatusEffectImmune_Module_Test test_case_statusType_50("TestData_StatusType_50_Percent");
    EXPECT_EQ(test_case_statusType_50.get_module_name(), "TestData_StatusType_50_Percent");
    EXPECT_TRUE(test_case_statusType_50.init_module(test_statusType_50));
    EXPECT_FALSE(test_case_statusType_50.test_get_status_effect_name().has_value());
    EXPECT_TRUE(test_case_statusType_50.test_get_status_effect_type().has_value());
    EXPECT_EQ(test_case_statusType_50.test_get_status_effect_type().value(), Status::StatusEffectType::POISON);
    EXPECT_TRUE(test_case_statusType_50.test_get_percent_chance_amount().has_value());
    EXPECT_FLOAT_EQ(test_case_statusType_50.test_get_percent_chance_amount().value(), 0.5f);

    // Probability is always a pain to test. In this test there are 3 runs, in each run, we apply the 
    //  damage 100 times, if it is removed between 40% and 60% in one of the runs it is deemed a success.
    //  Since the chance is 50% to remove it each time, it could be processed 1000 times all with the 
    //  same results and still be valid, just really unlikely. Probability is a pain to test.
    bool passed = false;
    for (int i = 0; i < 3; ++i) {
        int times_removed = 0;
        for (int j = 0; j < 100; ++j) {
            Damage test_output;
            std::string* value = test_output.add_status_effect();
            ASSERT_TRUE(value != nullptr);
            *value = "Ignite_V1";
            value = test_output.add_status_effect();
            ASSERT_TRUE(value != nullptr);
            *value = "Venom_V1";
            test_case_statusType_50.process_damage(test_output);
            EXPECT_EQ(test_output.damage_type(), Damage::NONE);
            EXPECT_FLOAT_EQ(test_output.amount(), 0.0f);
            EXPECT_EQ(test_output.status_effect_size(), 2);
            EXPECT_EQ(test_output.status_effect(0), "Ignite_V1");
            if (test_output.status_effect(1) == "") {
                ++times_removed;
            } else {
                EXPECT_EQ(test_output.status_effect(1), "Venom_V1");
            }
        }
        if (times_removed >= 40 && times_removed <= 60) {
            passed = true;
            break;
        }
    }
    EXPECT_TRUE(passed);

    // Healing should not be affected
    Heal test_heal;
    test_heal.set_type(Heal::POTION);
    test_heal.set_amount(3.2f);
    test_case_statusType_50.process_heal(test_heal);
    EXPECT_EQ(test_heal.type(), Heal::POTION);
    EXPECT_FLOAT_EQ(test_heal.amount(), 3.2f);

    Status::StatusEffectLibrary::get_Instance()->shutdown();
}

TEST(StatusEffectImmune_Module, Test_StatusEffectImmuneModule_StatusType_NegativePercent) {
    Status::StatusEffectLibrary::get_Instance()->init_from_file(k_yaml_library_file);

    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_statusType_NegativePercent = modules["TestData_StatusType_Negative_Percent"];
    EXPECT_TRUE(test_statusType_NegativePercent);
    EXPECT_TRUE(test_statusType_NegativePercent.IsMap());
    StatusEffectImmune_Module_Test test_case_statusType_NegativePercent("TestData_StatusType_Negative_Percent");
    EXPECT_EQ(test_case_statusType_NegativePercent.get_module_name(), "TestData_StatusType_Negative_Percent");
    EXPECT_FALSE(test_case_statusType_NegativePercent.init_module(test_statusType_NegativePercent));
    EXPECT_FALSE(test_case_statusType_NegativePercent.test_get_status_effect_name().has_value());
    EXPECT_FALSE(test_case_statusType_NegativePercent.test_get_status_effect_type().has_value());
    EXPECT_FALSE(test_case_statusType_NegativePercent.test_get_percent_chance_amount().has_value());

    // Nothing happens
    Damage test_output;
    std::string* value = test_output.add_status_effect();
    ASSERT_TRUE(value != nullptr);
    *value = "Ignite_V1";
    test_case_statusType_NegativePercent.process_damage(test_output);
    EXPECT_EQ(test_output.damage_type(), Damage::NONE);
    EXPECT_FLOAT_EQ(test_output.amount(), 0.0f);
    EXPECT_EQ(test_output.status_effect_size(), 1);
    EXPECT_EQ(test_output.status_effect(0), "Ignite_V1");

    Damage test_output_two;
    test_output_two.set_damage_type(Damage::FIRE);
    test_output_two.set_amount(3.2f);
    value = test_output_two.add_status_effect();
    ASSERT_TRUE(value != nullptr);
    *value = "Ignite_V1";
    value = test_output_two.add_status_effect();
    ASSERT_TRUE(value != nullptr);
    *value = "Foobar_V7";
    value = test_output_two.add_status_effect();
    ASSERT_TRUE(value != nullptr);
    *value = "Venom_V1";
    test_case_statusType_NegativePercent.process_damage(test_output_two);
    EXPECT_EQ(test_output_two.damage_type() & Damage::FIRE, Damage::FIRE);
    EXPECT_FLOAT_EQ(test_output_two.amount(), 3.2f);
    EXPECT_EQ(test_output_two.status_effect_size(), 3);
    EXPECT_EQ(test_output_two.status_effect(0), "Ignite_V1");
    EXPECT_EQ(test_output_two.status_effect(1), "Foobar_V7");
    EXPECT_EQ(test_output_two.status_effect(2), "Venom_V1");

    Heal test_heal;
    test_heal.set_type(Heal::POTION);
    test_heal.set_amount(3.2f);
    test_case_statusType_NegativePercent.process_heal(test_heal);
    EXPECT_EQ(test_heal.type(), Heal::POTION);
    EXPECT_FLOAT_EQ(test_heal.amount(), 3.2f);

    Status::StatusEffectLibrary::get_Instance()->shutdown();
}

TEST(StatusEffectImmune_Module, Test_StatusEffectImmuneModule_StatusType_TooMuchPercent) {
    Status::StatusEffectLibrary::get_Instance()->init_from_file(k_yaml_library_file);

    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_statusType_TooMuchPercent = modules["TestData_StatusType_TooMuch_Percent"];
    EXPECT_TRUE(test_statusType_TooMuchPercent);
    EXPECT_TRUE(test_statusType_TooMuchPercent.IsMap());
    StatusEffectImmune_Module_Test test_case_statusType_TooMuchPercent("TestData_StatusType_TooMuch_Percent");
    EXPECT_EQ(test_case_statusType_TooMuchPercent.get_module_name(), "TestData_StatusType_TooMuch_Percent");
    EXPECT_FALSE(test_case_statusType_TooMuchPercent.init_module(test_statusType_TooMuchPercent));
    EXPECT_FALSE(test_case_statusType_TooMuchPercent.test_get_status_effect_name().has_value());
    EXPECT_FALSE(test_case_statusType_TooMuchPercent.test_get_status_effect_type().has_value());
    EXPECT_FALSE(test_case_statusType_TooMuchPercent.test_get_percent_chance_amount().has_value());

    // Nothing happens
    Damage test_output;
    std::string* value = test_output.add_status_effect();
    ASSERT_TRUE(value != nullptr);
    *value = "Ignite_V1";
    test_case_statusType_TooMuchPercent.process_damage(test_output);
    EXPECT_EQ(test_output.damage_type(), Damage::NONE);
    EXPECT_FLOAT_EQ(test_output.amount(), 0.0f);
    EXPECT_EQ(test_output.status_effect_size(), 1);
    EXPECT_EQ(test_output.status_effect(0), "Ignite_V1");

    Damage test_output_two;
    test_output_two.set_damage_type(Damage::FIRE);
    test_output_two.set_amount(3.2f);
    value = test_output_two.add_status_effect();
    ASSERT_TRUE(value != nullptr);
    *value = "Ignite_V1";
    value = test_output_two.add_status_effect();
    ASSERT_TRUE(value != nullptr);
    *value = "Foobar_V7";
    value = test_output_two.add_status_effect();
    ASSERT_TRUE(value != nullptr);
    *value = "Venom_V1";
    test_case_statusType_TooMuchPercent.process_damage(test_output_two);
    EXPECT_EQ(test_output_two.damage_type() & Damage::FIRE, Damage::FIRE);
    EXPECT_FLOAT_EQ(test_output_two.amount(), 3.2f);
    EXPECT_EQ(test_output_two.status_effect_size(), 3);
    EXPECT_EQ(test_output_two.status_effect(0), "Ignite_V1");
    EXPECT_EQ(test_output_two.status_effect(1), "Foobar_V7");
    EXPECT_EQ(test_output_two.status_effect(2), "Venom_V1");

    Heal test_heal;
    test_heal.set_type(Heal::POTION);
    test_heal.set_amount(3.2f);
    test_case_statusType_TooMuchPercent.process_heal(test_heal);
    EXPECT_EQ(test_heal.type(), Heal::POTION);
    EXPECT_FLOAT_EQ(test_heal.amount(), 3.2f);

    Status::StatusEffectLibrary::get_Instance()->shutdown();
}

TEST(StatusEffectImmune_Module, Test_StatusEffectImmuneModule_StatusType_ZeroPercent) {
    Status::StatusEffectLibrary::get_Instance()->init_from_file(k_yaml_library_file);

    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_statusType_ZeroPercent = modules["TestData_StatusType_Zero_Percent"];
    EXPECT_TRUE(test_statusType_ZeroPercent);
    EXPECT_TRUE(test_statusType_ZeroPercent.IsMap());
    StatusEffectImmune_Module_Test test_case_statusType_ZeroPercent("TestData_StatusType_Zero_Percent");
    EXPECT_EQ(test_case_statusType_ZeroPercent.get_module_name(), "TestData_StatusType_Zero_Percent");
    EXPECT_FALSE(test_case_statusType_ZeroPercent.init_module(test_statusType_ZeroPercent));
    EXPECT_FALSE(test_case_statusType_ZeroPercent.test_get_status_effect_name().has_value());
    EXPECT_FALSE(test_case_statusType_ZeroPercent.test_get_status_effect_type().has_value());
    EXPECT_FALSE(test_case_statusType_ZeroPercent.test_get_percent_chance_amount().has_value());

    // Nothing happens
    Damage test_output;
    std::string* value = test_output.add_status_effect();
    ASSERT_TRUE(value != nullptr);
    *value = "Ignite_V1";
    test_case_statusType_ZeroPercent.process_damage(test_output);
    EXPECT_EQ(test_output.damage_type(), Damage::NONE);
    EXPECT_FLOAT_EQ(test_output.amount(), 0.0f);
    EXPECT_EQ(test_output.status_effect_size(), 1);
    EXPECT_EQ(test_output.status_effect(0), "Ignite_V1");

    Damage test_output_two;
    test_output_two.set_damage_type(Damage::FIRE);
    test_output_two.set_amount(3.2f);
    value = test_output_two.add_status_effect();
    ASSERT_TRUE(value != nullptr);
    *value = "Ignite_V1";
    value = test_output_two.add_status_effect();
    ASSERT_TRUE(value != nullptr);
    *value = "Foobar_V7";
    value = test_output_two.add_status_effect();
    ASSERT_TRUE(value != nullptr);
    *value = "Venom_V1";
    test_case_statusType_ZeroPercent.process_damage(test_output_two);
    EXPECT_EQ(test_output_two.damage_type() & Damage::FIRE, Damage::FIRE);
    EXPECT_FLOAT_EQ(test_output_two.amount(), 3.2f);
    EXPECT_EQ(test_output_two.status_effect_size(), 3);
    EXPECT_EQ(test_output_two.status_effect(0), "Ignite_V1");
    EXPECT_EQ(test_output_two.status_effect(1), "Foobar_V7");
    EXPECT_EQ(test_output_two.status_effect(2), "Venom_V1");

    Heal test_heal;
    test_heal.set_type(Heal::POTION);
    test_heal.set_amount(3.2f);
    test_case_statusType_ZeroPercent.process_heal(test_heal);
    EXPECT_EQ(test_heal.type(), Heal::POTION);
    EXPECT_FLOAT_EQ(test_heal.amount(), 3.2f);

    Status::StatusEffectLibrary::get_Instance()->shutdown();
}

TEST(StatusEffectImmune_Module, Test_StatusEffectImmuneModule_StatusType_StatusName) {
    Status::StatusEffectLibrary::get_Instance()->init_from_file(k_yaml_library_file);

    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_StatusName = modules["TestData_StatusName"];
    EXPECT_TRUE(test_StatusName);
    EXPECT_TRUE(test_StatusName.IsMap());
    StatusEffectImmune_Module_Test test_case_StatusName("TestData_StatusName");
    EXPECT_EQ(test_case_StatusName.get_module_name(), "TestData_StatusName");
    EXPECT_FALSE(test_case_StatusName.init_module(test_StatusName));
    EXPECT_FALSE(test_case_StatusName.test_get_status_effect_name().has_value());
    EXPECT_FALSE(test_case_StatusName.test_get_status_effect_type().has_value());
    EXPECT_FALSE(test_case_StatusName.test_get_percent_chance_amount().has_value());

    // Nothing happens
    Damage test_output;
    std::string* value = test_output.add_status_effect();
    ASSERT_TRUE(value != nullptr);
    *value = "Ignite_V1";
    test_case_StatusName.process_damage(test_output);
    EXPECT_EQ(test_output.damage_type(), Damage::NONE);
    EXPECT_FLOAT_EQ(test_output.amount(), 0.0f);
    EXPECT_EQ(test_output.status_effect_size(), 1);
    EXPECT_EQ(test_output.status_effect(0), "Ignite_V1");

    Damage test_output_two;
    test_output_two.set_damage_type(Damage::FIRE);
    test_output_two.set_amount(3.2f);
    value = test_output_two.add_status_effect();
    ASSERT_TRUE(value != nullptr);
    *value = "Ignite_V1";
    value = test_output_two.add_status_effect();
    ASSERT_TRUE(value != nullptr);
    *value = "Foobar_V7";
    value = test_output_two.add_status_effect();
    ASSERT_TRUE(value != nullptr);
    *value = "Venom_V1";
    test_case_StatusName.process_damage(test_output_two);
    EXPECT_EQ(test_output_two.damage_type() & Damage::FIRE, Damage::FIRE);
    EXPECT_FLOAT_EQ(test_output_two.amount(), 3.2f);
    EXPECT_EQ(test_output_two.status_effect_size(), 3);
    EXPECT_EQ(test_output_two.status_effect(0), "Ignite_V1");
    EXPECT_EQ(test_output_two.status_effect(1), "Foobar_V7");
    EXPECT_EQ(test_output_two.status_effect(2), "Venom_V1");

    Heal test_heal;
    test_heal.set_type(Heal::POTION);
    test_heal.set_amount(3.2f);
    test_case_StatusName.process_heal(test_heal);
    EXPECT_EQ(test_heal.type(), Heal::POTION);
    EXPECT_FLOAT_EQ(test_heal.amount(), 3.2f);

    Status::StatusEffectLibrary::get_Instance()->shutdown();
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
