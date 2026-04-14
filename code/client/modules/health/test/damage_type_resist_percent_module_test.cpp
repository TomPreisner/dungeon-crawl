#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include "code/client/damage/proto/damage.pb.h"
#include "code/client/health/proto/heal.pb.h"
#include "code/client/modules/health/damage_type_resist_percent_module.h"

// This Test class is just being used to expose some data to the 
//  public for use in the Test functions. These are not meant to be available
//  in the normal api, so they are being wrapped in this class instead.
class DamageTypeResistPercent_Module_Test : public Module::DamageTypeResistPercent_Module {
public:
    explicit DamageTypeResistPercent_Module_Test(const std::string& name) : DamageTypeResistPercent_Module(name) {}
    
    std::optional<int32_t> test_get_damage_type() { return m_damage_type; }
    std::optional<float> test_get_amount() { return m_amount; }
    std::optional<float> test_get_partial_effect_amount() { return m_partial_effect_amount; }
};

std::string k_yaml_file = "code\\client\\modules\\health\\test\\data\\test_damage_type_resist_module.yaml";

TEST(DamageTypeResistPercent_Module, Test_DamageTypeResistPercentModule_Simple_Apply) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_simple = modules["TestData_SimplePercent"];
    EXPECT_TRUE(test_simple);
    EXPECT_TRUE(test_simple.IsMap());
    DamageTypeResistPercent_Module_Test test_case_simple("TestData_SimplePercent");
    EXPECT_EQ(test_case_simple.get_module_name(), "TestData_SimplePercent");
    EXPECT_TRUE(test_case_simple.init_module(test_simple));
    EXPECT_TRUE(test_case_simple.test_get_damage_type().has_value());
    EXPECT_EQ(test_case_simple.test_get_damage_type().value() & Damage::PHYSICAL, Damage::PHYSICAL);
    EXPECT_TRUE(test_case_simple.test_get_amount().has_value());
    EXPECT_FLOAT_EQ(test_case_simple.test_get_amount().value(), 0.8f);
    EXPECT_FALSE(test_case_simple.test_get_partial_effect_amount().has_value());

    Damage test_output;
    test_output.set_damage_type(Damage::PHYSICAL);
    test_output.set_amount(3.2f);
    test_case_simple.process_damage(test_output);
    EXPECT_EQ(test_output.damage_type() & Damage::PHYSICAL, Damage::PHYSICAL);
    EXPECT_FLOAT_EQ(test_output.amount(), 0.64f);

    Damage test_output_two;
    test_output_two.set_damage_type(Damage::FIRE);
    test_output_two.set_amount(3.2f);
    test_case_simple.process_damage(test_output_two);
    EXPECT_EQ(test_output_two.damage_type() & Damage::FIRE, Damage::FIRE);
    EXPECT_FLOAT_EQ(test_output_two.amount(), 3.2f);

    // Healing should not be affected
    Heal test_heal;
    test_heal.set_type(Heal::POTION);
    test_heal.set_amount(3.2f);
    test_case_simple.process_heal(test_heal);
    EXPECT_EQ(test_heal.type(), Heal::POTION);
    EXPECT_FLOAT_EQ(test_heal.amount(), 3.2f);
}

TEST(DamageTypeResistPercent_Module, Test_DamageTypeResistPercentModule_SimplePartial_Apply) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());
    
    YAML::Node test_simple_partial = modules["TestData_SimplePercent_Partial"];
    EXPECT_TRUE(test_simple_partial);
    EXPECT_TRUE(test_simple_partial.IsMap());
    DamageTypeResistPercent_Module_Test test_case_simple_partial("TestData_SimplePercent_Partial");
    EXPECT_EQ(test_case_simple_partial.get_module_name(), "TestData_SimplePercent_Partial");
    EXPECT_TRUE(test_case_simple_partial.init_module(test_simple_partial));
    EXPECT_TRUE(test_case_simple_partial.test_get_damage_type().has_value());
    EXPECT_EQ(test_case_simple_partial.test_get_damage_type().value() & Damage::POISON, Damage::POISON);
    EXPECT_TRUE(test_case_simple_partial.test_get_amount().has_value());
    EXPECT_FLOAT_EQ(test_case_simple_partial.test_get_amount().value(), 0.8f);
    EXPECT_TRUE(test_case_simple_partial.test_get_partial_effect_amount().has_value());
    EXPECT_FLOAT_EQ(test_case_simple_partial.test_get_partial_effect_amount().value(), 0.4f);

    Damage test_output;
    test_output.set_damage_type(Damage::PHYSICAL | Damage::POISON);
    test_output.set_amount(3.2f);
    test_case_simple_partial.process_damage(test_output);
    EXPECT_EQ(test_output.damage_type() & Damage::PHYSICAL, Damage::PHYSICAL);
    EXPECT_EQ(test_output.damage_type() & Damage::POISON, Damage::POISON);
    EXPECT_FLOAT_EQ(test_output.amount(), 1.92f);

    Damage test_output_two;
    test_output_two.set_damage_type(Damage::FIRE);
    test_output_two.set_amount(3.2f);
    test_case_simple_partial.process_damage(test_output_two);
    EXPECT_EQ(test_output_two.damage_type() & Damage::FIRE, Damage::FIRE);
    EXPECT_FLOAT_EQ(test_output_two.amount(), 3.2f);

    Damage test_output_three;
    test_output_three.set_damage_type(Damage::POISON);
    test_output_three.set_amount(3.2f);
    test_case_simple_partial.process_damage(test_output_three);
    EXPECT_EQ(test_output_three.damage_type() & Damage::POISON, Damage::POISON);
    EXPECT_FLOAT_EQ(test_output_three.amount(), 0.64f);

    // Healing should not be affected
    Heal test_heal;
    test_heal.set_type(Heal::POTION);
    test_heal.set_amount(3.2f);
    test_case_simple_partial.process_heal(test_heal);
    EXPECT_EQ(test_heal.type(), Heal::POTION);
    EXPECT_FLOAT_EQ(test_heal.amount(), 3.2f);
}

TEST(DamageTypeResistPercent_Module, Test_DamageTypeResistPercentModule_MultipleDamage_Apply) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_multi_damage = modules["TestData_MultipleDamagePercent"];
    EXPECT_TRUE(test_multi_damage);
    EXPECT_TRUE(test_multi_damage.IsMap());
    DamageTypeResistPercent_Module_Test test_case_multi_damage("TestData_MultipleDamagePercent");
    EXPECT_EQ(test_case_multi_damage.get_module_name(), "TestData_MultipleDamagePercent");
    EXPECT_TRUE(test_case_multi_damage.init_module(test_multi_damage));
    EXPECT_TRUE(test_case_multi_damage.test_get_damage_type().has_value());
    EXPECT_EQ(test_case_multi_damage.test_get_damage_type().value() & Damage::MAGIC, Damage::MAGIC);
    EXPECT_EQ(test_case_multi_damage.test_get_damage_type().value() & Damage::FIRE, Damage::FIRE);
    EXPECT_TRUE(test_case_multi_damage.test_get_amount().has_value());
    EXPECT_FLOAT_EQ(test_case_multi_damage.test_get_amount().value(), 0.8f);
    EXPECT_TRUE(test_case_multi_damage.test_get_partial_effect_amount().has_value());
    EXPECT_FLOAT_EQ(test_case_multi_damage.test_get_partial_effect_amount().value(), 0.3f);

    Damage test_output;
    test_output.set_damage_type(Damage::MAGIC | Damage::FIRE);
    test_output.set_amount(3.2f);
    test_case_multi_damage.process_damage(test_output);
    EXPECT_EQ(test_output.damage_type() & Damage::MAGIC, Damage::MAGIC);
    EXPECT_EQ(test_output.damage_type() & Damage::FIRE, Damage::FIRE);
    EXPECT_FLOAT_EQ(test_output.amount(), 0.64f);

    Damage test_output_two;
    test_output_two.set_damage_type(Damage::MAGIC);
    test_output_two.set_amount(3.2f);
    test_case_multi_damage.process_damage(test_output_two);
    EXPECT_EQ(test_output_two.damage_type() & Damage::MAGIC, Damage::MAGIC);
    EXPECT_FLOAT_EQ(test_output_two.amount(), 0.64f);
    
    Damage test_output_three;
    test_output_three.set_damage_type(Damage::FIRE);
    test_output_three.set_amount(3.2f);
    test_case_multi_damage.process_damage(test_output_three);
    EXPECT_EQ(test_output_three.damage_type() & Damage::FIRE, Damage::FIRE);
    EXPECT_FLOAT_EQ(test_output_three.amount(), 0.64f);
    
    Damage test_output_four;
    test_output_four.set_damage_type(Damage::MAGIC | Damage::FIRE | Damage::POISON);
    test_output_four.set_amount(3.2f);
    test_case_multi_damage.process_damage(test_output_four);
    EXPECT_EQ(test_output_four.damage_type() & Damage::MAGIC, Damage::MAGIC);
    EXPECT_EQ(test_output_four.damage_type() & Damage::FIRE, Damage::FIRE);
    EXPECT_EQ(test_output_four.damage_type() & Damage::POISON, Damage::POISON);
    EXPECT_FLOAT_EQ(test_output_four.amount(), 2.24f);
    
    Damage test_output_five;
    test_output_five.set_damage_type(Damage::POISON);
    test_output_five.set_amount(3.2f);
    test_case_multi_damage.process_damage(test_output_five);
    EXPECT_EQ(test_output_five.damage_type() & Damage::POISON, Damage::POISON);
    EXPECT_FLOAT_EQ(test_output_five.amount(), 3.2f);

    // Healing should not be affected
    Heal test_heal;
    test_heal.set_type(Heal::POTION);
    test_heal.set_amount(3.2f);
    test_case_multi_damage.process_heal(test_heal);
    EXPECT_EQ(test_heal.type(), Heal::POTION);
    EXPECT_FLOAT_EQ(test_heal.amount(), 3.2f);
}

TEST(DamageTypeResistPercent_Module, Test_DamageTypeResistPercentModule_MultipleDamage_BadPartial_Apply) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());
    
    YAML::Node test_multi_damage_partial = modules["TestData_MultipleDamagePercent_BadPartial"];
    EXPECT_TRUE(test_multi_damage_partial);
    EXPECT_TRUE(test_multi_damage_partial.IsMap());
    DamageTypeResistPercent_Module_Test test_case_multi_damage_partial("TestData_MultipleDamagePercent_BadPartial");
    EXPECT_EQ(test_case_multi_damage_partial.get_module_name(), "TestData_MultipleDamagePercent_BadPartial");
    EXPECT_FALSE(test_case_multi_damage_partial.init_module(test_multi_damage_partial));
    EXPECT_FALSE(test_case_multi_damage_partial.test_get_damage_type().has_value());
    EXPECT_FALSE(test_case_multi_damage_partial.test_get_amount().has_value());
    EXPECT_FALSE(test_case_multi_damage_partial.test_get_partial_effect_amount().has_value());
    
    Damage test_output;
    test_output.set_damage_type(Damage::MAGIC | Damage::FIRE);
    test_output.set_amount(3.2f);
    test_case_multi_damage_partial.process_damage(test_output);
    EXPECT_EQ(test_output.damage_type() & Damage::MAGIC, Damage::MAGIC);
    EXPECT_EQ(test_output.damage_type() & Damage::FIRE, Damage::FIRE);
    EXPECT_FLOAT_EQ(test_output.amount(), 3.2f);

    Damage test_output_two;
    test_output_two.set_damage_type(Damage::MAGIC);
    test_output_two.set_amount(3.2f);
    test_case_multi_damage_partial.process_damage(test_output_two);
    EXPECT_EQ(test_output_two.damage_type() & Damage::MAGIC, Damage::MAGIC);
    EXPECT_FLOAT_EQ(test_output_two.amount(), 3.2f);
    
    Damage test_output_three;
    test_output_three.set_damage_type(Damage::FIRE);
    test_output_three.set_amount(3.2f);
    test_case_multi_damage_partial.process_damage(test_output_three);
    EXPECT_EQ(test_output_three.damage_type() & Damage::FIRE, Damage::FIRE);
    EXPECT_FLOAT_EQ(test_output_three.amount(), 3.2f);
    
    Damage test_output_four;
    test_output_four.set_damage_type(Damage::MAGIC | Damage::FIRE | Damage::POISON);
    test_output_four.set_amount(3.2f);
    test_case_multi_damage_partial.process_damage(test_output_four);
    EXPECT_EQ(test_output_four.damage_type() & Damage::MAGIC, Damage::MAGIC);
    EXPECT_EQ(test_output_four.damage_type() & Damage::FIRE, Damage::FIRE);
    EXPECT_EQ(test_output_four.damage_type() & Damage::POISON, Damage::POISON);
    EXPECT_FLOAT_EQ(test_output_four.amount(), 3.2f);
    
    Damage test_output_five;
    test_output_five.set_damage_type(Damage::POISON);
    test_output_five.set_amount(3.2f);
    test_case_multi_damage_partial.process_damage(test_output_five);
    EXPECT_EQ(test_output_five.damage_type() & Damage::POISON, Damage::POISON);
    EXPECT_FLOAT_EQ(test_output_five.amount(), 3.2f);

    // Healing should not be affected
    Heal test_heal;
    test_heal.set_type(Heal::POTION);
    test_heal.set_amount(3.2f);
    test_case_multi_damage_partial.process_heal(test_heal);
    EXPECT_EQ(test_heal.type(), Heal::POTION);
    EXPECT_FLOAT_EQ(test_heal.amount(), 3.2f);
}

TEST(DamageTypeResistPercent_Module, Test_DamageTypeResistPercentModule_NoDamage_Apply) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_no_damage = modules["TestData_NoDamage"];
    EXPECT_TRUE(test_no_damage);
    EXPECT_TRUE(test_no_damage.IsMap());
    DamageTypeResistPercent_Module_Test test_case_no_damage("TestData_NoDamage");
    EXPECT_EQ(test_case_no_damage.get_module_name(), "TestData_NoDamage");
    EXPECT_FALSE(test_case_no_damage.init_module(test_no_damage));
    EXPECT_FALSE(test_case_no_damage.test_get_damage_type().has_value());
    EXPECT_FALSE(test_case_no_damage.test_get_amount().has_value());
    EXPECT_FALSE(test_case_no_damage.test_get_partial_effect_amount().has_value());

    // Nothing happens
    Damage test_output;
    test_output.set_damage_type(Damage::FIRE);
    test_output.set_amount(3.2f);
    test_case_no_damage.process_damage(test_output);
    EXPECT_EQ(test_output.damage_type(), Damage::FIRE);
    EXPECT_FLOAT_EQ(test_output.amount(), 3.2f);

    Heal test_heal;
    test_heal.set_type(Heal::POTION);
    test_heal.set_amount(3.2f);
    test_case_no_damage.process_heal(test_heal);
    EXPECT_EQ(test_heal.type(), Heal::POTION);
    EXPECT_FLOAT_EQ(test_heal.amount(), 3.2f);
}

TEST(DamageTypeResistPercent_Module, Test_DamageTypeResistPercentModule_NoAmount_Apply) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_no_amount = modules["TestData_NoAmount"];
    EXPECT_TRUE(test_no_amount);
    EXPECT_TRUE(test_no_amount.IsMap());
    DamageTypeResistPercent_Module_Test test_case_no_amount("TestData_NoAmount");
    EXPECT_EQ(test_case_no_amount.get_module_name(), "TestData_NoAmount");
    EXPECT_FALSE(test_case_no_amount.init_module(test_no_amount));
    EXPECT_FALSE(test_case_no_amount.test_get_damage_type().has_value());
    EXPECT_FALSE(test_case_no_amount.test_get_amount().has_value());
    EXPECT_FALSE(test_case_no_amount.test_get_partial_effect_amount().has_value());

    // Nothing happens
    Damage test_output;
    test_output.set_damage_type(Damage::FIRE);
    test_output.set_amount(3.2f);
    test_case_no_amount.process_damage(test_output);
    EXPECT_EQ(test_output.damage_type(), Damage::FIRE);
    EXPECT_FLOAT_EQ(test_output.amount(), 3.2f);

    Heal test_heal;
    test_heal.set_type(Heal::POTION);
    test_heal.set_amount(3.2f);
    test_case_no_amount.process_heal(test_heal);
    EXPECT_EQ(test_heal.type(), Heal::POTION);
    EXPECT_FLOAT_EQ(test_heal.amount(), 3.2f);
}

TEST(DamageTypeResistPercent_Module, Test_DamageTypeResistPercentModule_NegativeAmount_Apply) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_negative_amount = modules["TestData_NegativeAmount"];
    EXPECT_TRUE(test_negative_amount);
    EXPECT_TRUE(test_negative_amount.IsMap());
    DamageTypeResistPercent_Module_Test test_case_negative_amount("TestData_NegativeAmount");
    EXPECT_EQ(test_case_negative_amount.get_module_name(), "TestData_NegativeAmount");
    EXPECT_FALSE(test_case_negative_amount.init_module(test_negative_amount));
    EXPECT_FALSE(test_case_negative_amount.test_get_damage_type().has_value());
    EXPECT_FALSE(test_case_negative_amount.test_get_amount().has_value());
    EXPECT_FALSE(test_case_negative_amount.test_get_partial_effect_amount().has_value());

    // Nothing happens
    Damage test_output;
    test_output.set_damage_type(Damage::FIRE);
    test_output.set_amount(3.2f);
    test_case_negative_amount.process_damage(test_output);
    EXPECT_EQ(test_output.damage_type(), Damage::FIRE);
    EXPECT_FLOAT_EQ(test_output.amount(), 3.2f);

    Heal test_heal;
    test_heal.set_type(Heal::POTION);
    test_heal.set_amount(3.2f);
    test_case_negative_amount.process_heal(test_heal);
    EXPECT_EQ(test_heal.type(), Heal::POTION);
    EXPECT_FLOAT_EQ(test_heal.amount(), 3.2f);
}

TEST(DamageTypeResistPercent_Module, Test_DamageTypeResistPercentModule_PercentTooLarge_Apply) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_percent_too_large = modules["TestData_PercentTooLarge"];
    EXPECT_TRUE(test_percent_too_large);
    EXPECT_TRUE(test_percent_too_large.IsMap());
    DamageTypeResistPercent_Module_Test test_case_percent_too_large("TestData_PercentTooLarge");
    EXPECT_EQ(test_case_percent_too_large.get_module_name(), "TestData_PercentTooLarge");
    EXPECT_FALSE(test_case_percent_too_large.init_module(test_percent_too_large));
    EXPECT_FALSE(test_case_percent_too_large.test_get_damage_type().has_value());
    EXPECT_FALSE(test_case_percent_too_large.test_get_amount().has_value());
    EXPECT_FALSE(test_case_percent_too_large.test_get_partial_effect_amount().has_value());

    // Nothing happens
    Damage test_output;
    test_output.set_damage_type(Damage::PHYSICAL);
    test_output.set_amount(3.2f);
    test_case_percent_too_large.process_damage(test_output);
    EXPECT_EQ(test_output.damage_type(), Damage::PHYSICAL);
    EXPECT_FLOAT_EQ(test_output.amount(), 3.2f);

    Damage test_output_two;
    test_output_two.set_damage_type(Damage::FIRE);
    test_output_two.set_amount(3.2f);
    test_case_percent_too_large.process_damage(test_output_two);
    EXPECT_EQ(test_output_two.damage_type(), Damage::FIRE);
    EXPECT_FLOAT_EQ(test_output_two.amount(), 3.2f);

    Heal test_heal;
    test_heal.set_type(Heal::POTION);
    test_heal.set_amount(3.2f);
    test_case_percent_too_large.process_heal(test_heal);
    EXPECT_EQ(test_heal.type(), Heal::POTION);
    EXPECT_FLOAT_EQ(test_heal.amount(), 3.2f);
}

TEST(DamageTypeResistPercent_Module, Test_DamageTypeResistPercentModule_PercentPartialNegative_Apply) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_percent_partial_negative = modules["TestData_PercentPartialNegative"];
    EXPECT_TRUE(test_percent_partial_negative);
    EXPECT_TRUE(test_percent_partial_negative.IsMap());
    DamageTypeResistPercent_Module_Test test_case_percent_partial_negative("TestData_PercentPartialNegative");
    EXPECT_EQ(test_case_percent_partial_negative.get_module_name(), "TestData_PercentPartialNegative");
    EXPECT_FALSE(test_case_percent_partial_negative.init_module(test_percent_partial_negative));
    EXPECT_FALSE(test_case_percent_partial_negative.test_get_damage_type().has_value());
    EXPECT_FALSE(test_case_percent_partial_negative.test_get_amount().has_value());
    EXPECT_FALSE(test_case_percent_partial_negative.test_get_partial_effect_amount().has_value());

    // Nothing happens
    Damage test_output;
    test_output.set_damage_type(Damage::PHYSICAL);
    test_output.set_amount(3.2f);
    test_case_percent_partial_negative.process_damage(test_output);
    EXPECT_EQ(test_output.damage_type(), Damage::PHYSICAL);
    EXPECT_FLOAT_EQ(test_output.amount(), 3.2f);

    Damage test_output_two;
    test_output_two.set_damage_type(Damage::FIRE);
    test_output_two.set_amount(3.2f);
    test_case_percent_partial_negative.process_damage(test_output_two);
    EXPECT_EQ(test_output_two.damage_type(), Damage::FIRE);
    EXPECT_FLOAT_EQ(test_output_two.amount(), 3.2f);

    Heal test_heal;
    test_heal.set_type(Heal::POTION);
    test_heal.set_amount(3.2f);
    test_case_percent_partial_negative.process_heal(test_heal);
    EXPECT_EQ(test_heal.type(), Heal::POTION);
    EXPECT_FLOAT_EQ(test_heal.amount(), 3.2f);
}

TEST(DamageTypeResistPercent_Module, Test_DamageTypeResistPercentModule_PercentPartialLargerThanAmount_Apply) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_percent_partial_larger_than_amount = modules["TestData_PercentPartialLargerThanAmount"];
    EXPECT_TRUE(test_percent_partial_larger_than_amount);
    EXPECT_TRUE(test_percent_partial_larger_than_amount.IsMap());
    DamageTypeResistPercent_Module_Test test_case_percent_partial_larger_than_amount("TestData_PercentPartialLargerThanAmount");
    EXPECT_EQ(test_case_percent_partial_larger_than_amount.get_module_name(), "TestData_PercentPartialLargerThanAmount");
    EXPECT_FALSE(test_case_percent_partial_larger_than_amount.init_module(test_percent_partial_larger_than_amount));
    EXPECT_FALSE(test_case_percent_partial_larger_than_amount.test_get_damage_type().has_value());
    EXPECT_FALSE(test_case_percent_partial_larger_than_amount.test_get_amount().has_value());
    EXPECT_FALSE(test_case_percent_partial_larger_than_amount.test_get_partial_effect_amount().has_value());

    // Nothing happens
    Damage test_output;
    test_output.set_damage_type(Damage::PHYSICAL);
    test_output.set_amount(3.2f);
    test_case_percent_partial_larger_than_amount.process_damage(test_output);
    EXPECT_EQ(test_output.damage_type(), Damage::PHYSICAL);
    EXPECT_FLOAT_EQ(test_output.amount(), 3.2f);

    Damage test_output_two;
    test_output_two.set_damage_type(Damage::FIRE);
    test_output_two.set_amount(3.2f);
    test_case_percent_partial_larger_than_amount.process_damage(test_output_two);
    EXPECT_EQ(test_output_two.damage_type(), Damage::FIRE);
    EXPECT_FLOAT_EQ(test_output_two.amount(), 3.2f);

    Heal test_heal;
    test_heal.set_type(Heal::POTION);
    test_heal.set_amount(3.2f);
    test_case_percent_partial_larger_than_amount.process_heal(test_heal);
    EXPECT_EQ(test_heal.type(), Heal::POTION);
    EXPECT_FLOAT_EQ(test_heal.amount(), 3.2f);
}

TEST(DamageTypeResistPercent_Module, Test_DamageTypeResistPercentModule_PercentPartialTooLarge_Apply) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_percent_partial_too_large = modules["TestData_PercentPartialTooLarge"];
    EXPECT_TRUE(test_percent_partial_too_large);
    EXPECT_TRUE(test_percent_partial_too_large.IsMap());
    DamageTypeResistPercent_Module_Test test_case_percent_partial_too_large("TestData_PercentPartialTooLarge");
    EXPECT_EQ(test_case_percent_partial_too_large.get_module_name(), "TestData_PercentPartialTooLarge");
    EXPECT_FALSE(test_case_percent_partial_too_large.init_module(test_percent_partial_too_large));
    EXPECT_FALSE(test_case_percent_partial_too_large.test_get_damage_type().has_value());
    EXPECT_FALSE(test_case_percent_partial_too_large.test_get_amount().has_value());
    EXPECT_FALSE(test_case_percent_partial_too_large.test_get_partial_effect_amount().has_value());

    // Nothing happens
    Damage test_output;
    test_output.set_damage_type(Damage::PHYSICAL);
    test_output.set_amount(3.2f);
    test_case_percent_partial_too_large.process_damage(test_output);
    EXPECT_EQ(test_output.damage_type(), Damage::PHYSICAL);
    EXPECT_FLOAT_EQ(test_output.amount(), 3.2f);

    Damage test_output_two;
    test_output_two.set_damage_type(Damage::FIRE);
    test_output_two.set_amount(3.2f);
    test_case_percent_partial_too_large.process_damage(test_output_two);
    EXPECT_EQ(test_output_two.damage_type(), Damage::FIRE);
    EXPECT_FLOAT_EQ(test_output_two.amount(), 3.2f);

    Heal test_heal;
    test_heal.set_type(Heal::POTION);
    test_heal.set_amount(3.2f);
    test_case_percent_partial_too_large.process_heal(test_heal);
    EXPECT_EQ(test_heal.type(), Heal::POTION);
    EXPECT_FLOAT_EQ(test_heal.amount(), 3.2f);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
