#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include "code/client/messages/proto/damage.pb.h"
#include "code/client/messages/proto/heal.pb.h"
#include "code/client/modules/descriptors/heal_type_amount_descriptor.h"
#include "code/client/modules/health/heal_type_increase_health_module.h"

// This Test class is just being used to expose some data to the 
//  public for use in the Test functions. These are not meant to be available
//  in the normal api, so they are being wrapped in this class instead.
class HealTypeIncrease_HealthModule_Test : public Module::HealTypeIncrease_HealthModule {
public:
    HealTypeIncrease_HealthModule_Test(const std::string& name, const YAML::Node& data) : Module::HealTypeIncrease_HealthModule(name, data) {}

    std::optional<code::client::messages::Heal::HealType> test_get_heal_type() {
        Module::HealTypeAmount_Descriptor* desc = get_descriptor<Module::HealTypeAmount_Descriptor>();
        if (desc != nullptr) {
            return desc->get_heal_type(); 
        }
        return std::nullopt;
    }

    std::optional<float> test_get_amount() { 
        Module::HealTypeAmount_Descriptor* desc = get_descriptor<Module::HealTypeAmount_Descriptor>();
        if (desc != nullptr) {
            return desc->get_amount(); 
        }
        return std::nullopt;
    }
};

std::string k_yaml_file = "code\\client\\modules\\health\\test\\data\\test_heal_type_increase_module.yaml";

TEST(HealTypeIncrease_HealthModule, Test_HealTypeIncreaseModule_Simple_Apply) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_simple = modules["TestData_Simple"];
    EXPECT_TRUE(test_simple);
    EXPECT_TRUE(test_simple.IsMap());
    HealTypeIncrease_HealthModule_Test test_case_simple("TestData_Simple", test_simple);
    EXPECT_EQ(test_case_simple.get_module_name(), "TestData_Simple");
    EXPECT_TRUE(test_case_simple.test_get_heal_type().has_value());
    EXPECT_EQ(test_case_simple.test_get_heal_type().value(), code::client::messages::Heal::POTION);
    EXPECT_TRUE(test_case_simple.test_get_amount().has_value());
    EXPECT_FLOAT_EQ(test_case_simple.test_get_amount().value(), 2.5f);

    code::client::messages::Heal test_heal;
    test_heal.set_type(code::client::messages::Heal::POTION);
    test_heal.set_amount(3.2f);
    test_case_simple.process_heal(test_heal);
    EXPECT_EQ(test_heal.type(), code::client::messages::Heal::POTION);
    EXPECT_FLOAT_EQ(test_heal.amount(), 5.7f);

    code::client::messages::Heal test_heal_two;
    test_heal_two.set_type(code::client::messages::Heal::MAGIC);
    test_heal_two.set_amount(4.2f);
    test_case_simple.process_heal(test_heal_two);
    EXPECT_EQ(test_heal_two.type(), code::client::messages::Heal::MAGIC);
    EXPECT_FLOAT_EQ(test_heal_two.amount(), 4.2f);

    // Damage should not be affected
    code::client::messages::Damage test_output;
    test_output.set_damage_type(code::client::messages::Damage::PHYSICAL);
    test_output.set_amount(3.2f);
    test_case_simple.process_damage(test_output);
    EXPECT_EQ(test_output.damage_type() & code::client::messages::Damage::PHYSICAL,
                code::client::messages::Damage::PHYSICAL);
    EXPECT_FLOAT_EQ(test_output.amount(), 3.2f);
}

TEST(HealTypeIncrease_HealthModule, Test_HealTypeIncreaseModule_NoHeal_Apply) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_no_heal = modules["TestData_NoHeal"];
    EXPECT_TRUE(test_no_heal);
    EXPECT_TRUE(test_no_heal.IsMap());
    HealTypeIncrease_HealthModule_Test test_case_no_heal("TestData_NoHeal", test_no_heal);
    EXPECT_EQ(test_case_no_heal.get_module_name(), "TestData_NoHeal");
    EXPECT_FALSE(test_case_no_heal.test_get_heal_type().has_value());
    EXPECT_FALSE(test_case_no_heal.test_get_amount().has_value());

    // Nothing happens
    code::client::messages::Heal test_heal;
    test_heal.set_type(code::client::messages::Heal::POTION);
    test_heal.set_amount(3.2f);
    test_case_no_heal.process_heal(test_heal);
    EXPECT_EQ(test_heal.type(), code::client::messages::Heal::POTION);
    EXPECT_FLOAT_EQ(test_heal.amount(), 3.2f);

    code::client::messages::Heal test_heal_two;
    test_heal_two.set_type(code::client::messages::Heal::MAGIC);
    test_heal_two.set_amount(4.2f);
    test_case_no_heal.process_heal(test_heal_two);
    EXPECT_EQ(test_heal_two.type(), code::client::messages::Heal::MAGIC);
    EXPECT_FLOAT_EQ(test_heal_two.amount(), 4.2f);

    code::client::messages::Damage test_output;
    test_output.set_damage_type(code::client::messages::Damage::PHYSICAL);
    test_output.set_amount(3.2f);
    test_case_no_heal.process_damage(test_output);
    EXPECT_EQ(test_output.damage_type() & code::client::messages::Damage::PHYSICAL,
                code::client::messages::Damage::PHYSICAL);
    EXPECT_FLOAT_EQ(test_output.amount(), 3.2f);
}

TEST(HealTypeIncrease_HealthModule, Test_HealTypeIncreaseModule_HealNotString_Apply) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_heal_not_string = modules["TestData_HealNotString"];
    EXPECT_TRUE(test_heal_not_string);
    EXPECT_TRUE(test_heal_not_string.IsMap());
    HealTypeIncrease_HealthModule_Test test_case_heal_not_string("TestData_HealNotString", test_heal_not_string);
    EXPECT_EQ(test_case_heal_not_string.get_module_name(), "TestData_HealNotString");
    EXPECT_FALSE(test_case_heal_not_string.test_get_heal_type().has_value());
    EXPECT_FALSE(test_case_heal_not_string.test_get_amount().has_value());

    // Nothing happens
    code::client::messages::Heal test_heal;
    test_heal.set_type(code::client::messages::Heal::POTION);
    test_heal.set_amount(3.2f);
    test_case_heal_not_string.process_heal(test_heal);
    EXPECT_EQ(test_heal.type(), code::client::messages::Heal::POTION);
    EXPECT_FLOAT_EQ(test_heal.amount(), 3.2f);

    code::client::messages::Heal test_heal_two;
    test_heal_two.set_type(code::client::messages::Heal::MAGIC);
    test_heal_two.set_amount(4.2f);
    test_case_heal_not_string.process_heal(test_heal_two);
    EXPECT_EQ(test_heal_two.type(), code::client::messages::Heal::MAGIC);
    EXPECT_FLOAT_EQ(test_heal_two.amount(), 4.2f);

    code::client::messages::Damage test_output;
    test_output.set_damage_type(code::client::messages::Damage::PHYSICAL);
    test_output.set_amount(3.2f);
    test_case_heal_not_string.process_damage(test_output);
    EXPECT_EQ(test_output.damage_type() & code::client::messages::Damage::PHYSICAL,
                code::client::messages::Damage::PHYSICAL);
    EXPECT_FLOAT_EQ(test_output.amount(), 3.2f);
}

TEST(HealTypeIncrease_HealthModule, Test_HealTypeIncreaseModule_HealInvalidType_Apply) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_heal_invalid_type = modules["TestData_HealInvalidType"];
    EXPECT_TRUE(test_heal_invalid_type);
    EXPECT_TRUE(test_heal_invalid_type.IsMap());
    HealTypeIncrease_HealthModule_Test test_case_heal_invalid_type("TestData_HealInvalidType", test_heal_invalid_type);
    EXPECT_EQ(test_case_heal_invalid_type.get_module_name(), "TestData_HealInvalidType");
    EXPECT_FALSE(test_case_heal_invalid_type.test_get_heal_type().has_value());
    EXPECT_FALSE(test_case_heal_invalid_type.test_get_amount().has_value());

    // Nothing happens
    code::client::messages::Heal test_heal;
    test_heal.set_type(code::client::messages::Heal::POTION);
    test_heal.set_amount(3.2f);
    test_case_heal_invalid_type.process_heal(test_heal);
    EXPECT_EQ(test_heal.type(), code::client::messages::Heal::POTION);
    EXPECT_FLOAT_EQ(test_heal.amount(), 3.2f);

    code::client::messages::Heal test_heal_two;
    test_heal_two.set_type(code::client::messages::Heal::MAGIC);
    test_heal_two.set_amount(4.2f);
    test_case_heal_invalid_type.process_heal(test_heal_two);
    EXPECT_EQ(test_heal_two.type(), code::client::messages::Heal::MAGIC);
    EXPECT_FLOAT_EQ(test_heal_two.amount(), 4.2f);

    code::client::messages::Damage test_output;
    test_output.set_damage_type(code::client::messages::Damage::PHYSICAL);
    test_output.set_amount(3.2f);
    test_case_heal_invalid_type.process_damage(test_output);
    EXPECT_EQ(test_output.damage_type() & code::client::messages::Damage::PHYSICAL,
                code::client::messages::Damage::PHYSICAL);
    EXPECT_FLOAT_EQ(test_output.amount(), 3.2f);
}

TEST(HealTypeIncrease_HealthModule, Test_HealTypeIncreaseModule_No_Amount_Apply) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_no_amount = modules["TestData_NoAmount"];
    EXPECT_TRUE(test_no_amount);
    EXPECT_TRUE(test_no_amount.IsMap());
    HealTypeIncrease_HealthModule_Test test_case_no_amount("TestData_NoAmount", test_no_amount);
    EXPECT_EQ(test_case_no_amount.get_module_name(), "TestData_NoAmount");
    EXPECT_FALSE(test_case_no_amount.test_get_heal_type().has_value());
    EXPECT_FALSE(test_case_no_amount.test_get_amount().has_value());

    // Nothing happens
    code::client::messages::Heal test_heal;
    test_heal.set_type(code::client::messages::Heal::POTION);
    test_heal.set_amount(3.2f);
    test_case_no_amount.process_heal(test_heal);
    EXPECT_EQ(test_heal.type(), code::client::messages::Heal::POTION);
    EXPECT_FLOAT_EQ(test_heal.amount(), 3.2f);

    code::client::messages::Heal test_heal_two;
    test_heal_two.set_type(code::client::messages::Heal::MAGIC);
    test_heal_two.set_amount(4.2f);
    test_case_no_amount.process_heal(test_heal_two);
    EXPECT_EQ(test_heal_two.type(), code::client::messages::Heal::MAGIC);
    EXPECT_FLOAT_EQ(test_heal_two.amount(), 4.2f);

    code::client::messages::Damage test_output;
    test_output.set_damage_type(code::client::messages::Damage::PHYSICAL);
    test_output.set_amount(3.2f);
    test_case_no_amount.process_damage(test_output);
    EXPECT_EQ(test_output.damage_type() & code::client::messages::Damage::PHYSICAL,
                code::client::messages::Damage::PHYSICAL);
    EXPECT_FLOAT_EQ(test_output.amount(), 3.2f);
}

TEST(HealTypeIncrease_HealthModule, Test_HealTypeIncreaseModule_Negative_Amount_Apply) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_negative_amount = modules["TestData_NegativeAmount"];
    EXPECT_TRUE(test_negative_amount);
    EXPECT_TRUE(test_negative_amount.IsMap());
    HealTypeIncrease_HealthModule_Test test_case_negative_amount("TestData_NegativeAmount", test_negative_amount);
    EXPECT_EQ(test_case_negative_amount.get_module_name(), "TestData_NegativeAmount");
    EXPECT_FALSE(test_case_negative_amount.test_get_heal_type().has_value());
    EXPECT_FALSE(test_case_negative_amount.test_get_amount().has_value());

    // Nothing happens
    code::client::messages::Heal test_heal;
    test_heal.set_type(code::client::messages::Heal::POTION);
    test_heal.set_amount(3.2f);
    test_case_negative_amount.process_heal(test_heal);
    EXPECT_EQ(test_heal.type(), code::client::messages::Heal::POTION);
    EXPECT_FLOAT_EQ(test_heal.amount(), 3.2f);

    code::client::messages::Heal test_heal_two;
    test_heal_two.set_type(code::client::messages::Heal::MAGIC);
    test_heal_two.set_amount(4.2f);
    test_case_negative_amount.process_heal(test_heal_two);
    EXPECT_EQ(test_heal_two.type(), code::client::messages::Heal::MAGIC);
    EXPECT_FLOAT_EQ(test_heal_two.amount(), 4.2f);

    code::client::messages::Damage test_output;
    test_output.set_damage_type(code::client::messages::Damage::PHYSICAL);
    test_output.set_amount(3.2f);
    test_case_negative_amount.process_damage(test_output);
    EXPECT_EQ(test_output.damage_type() & code::client::messages::Damage::PHYSICAL,
                code::client::messages::Damage::PHYSICAL);
    EXPECT_FLOAT_EQ(test_output.amount(), 3.2f);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
