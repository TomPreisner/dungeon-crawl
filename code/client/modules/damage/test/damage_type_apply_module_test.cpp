#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include "code/client/messages/proto/damage.pb.h"
#include "code/client/modules/damage/damage_type_apply_module.h"

// This Test class is just being used to expose some data to the 
//  public for use in the Test functions. These are not meant to be available
//  in the normal api, so they are being wrapped in this class instead.
class DamageTypeApply_Module_Test : public Module::DamageTypeApply_Module {
public:
    explicit DamageTypeApply_Module_Test(const std::string& name) : DamageTypeApply_Module(name) {}
    
    std::optional<int32_t> test_get_damage_type() { return m_damage_type; }
    std::optional<float> test_get_amount() { return m_amount; }
};

std::string k_yaml_file = "code\\client\\modules\\damage\\test\\data\\test_damage_type_apply_module.yaml";

TEST(DamageTypeApply_Module, Test_DamageTypeApplyModule_Apply_Simple) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_simple = modules["TestData_Simple"];
    EXPECT_TRUE(test_simple);
    EXPECT_TRUE(test_simple.IsMap());
    DamageTypeApply_Module_Test test_case_simple("TestData_Simple");
    EXPECT_EQ(test_case_simple.get_module_name(), "TestData_Simple");
    EXPECT_TRUE(test_case_simple.init_module(test_simple));
    EXPECT_TRUE(test_case_simple.test_get_damage_type().has_value());
    EXPECT_EQ(test_case_simple.test_get_damage_type().value() & code::client::messages::Damage::PHYSICAL,
                code::client::messages::Damage::PHYSICAL);
    EXPECT_TRUE(test_case_simple.test_get_amount().has_value());
    EXPECT_FLOAT_EQ(test_case_simple.test_get_amount().value(), 2.5f);

    code::client::messages::Damage test_output;
    test_case_simple.apply_damage(test_output);
    EXPECT_EQ(test_output.damage_type() & code::client::messages::Damage::PHYSICAL,
                code::client::messages::Damage::PHYSICAL);
    EXPECT_FLOAT_EQ(test_output.amount(), 2.5f);
    
    test_case_simple.apply_damage(test_output);
    EXPECT_EQ(test_output.damage_type() & code::client::messages::Damage::PHYSICAL,
                code::client::messages::Damage::PHYSICAL);
    EXPECT_FLOAT_EQ(test_output.amount(), 5.0f);

    code::client::messages::Damage test_output_two;
    test_output_two.set_damage_type(code::client::messages::Damage::FIRE);
    test_output_two.set_amount(3.2f);
    test_case_simple.apply_damage(test_output_two);
    EXPECT_EQ(test_output_two.damage_type() & code::client::messages::Damage::PHYSICAL,
                code::client::messages::Damage::PHYSICAL);
    EXPECT_EQ(test_output_two.damage_type() & code::client::messages::Damage::FIRE,
                code::client::messages::Damage::FIRE);
    EXPECT_FLOAT_EQ(test_output_two.amount(), 5.7f);
}

TEST(DamageTypeApply_Module, Test_DamageTypeApplyModule_Apply_MultipleDamage) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_multi_damage = modules["TestData_MultipleDamage"];
    EXPECT_TRUE(test_multi_damage);
    EXPECT_TRUE(test_multi_damage.IsMap());
    DamageTypeApply_Module_Test test_case_multi_damage("TestData_MultipleDamage");
    EXPECT_EQ(test_case_multi_damage.get_module_name(), "TestData_MultipleDamage");
    EXPECT_TRUE(test_case_multi_damage.init_module(test_multi_damage));
    EXPECT_TRUE(test_case_multi_damage.test_get_damage_type().has_value());
    EXPECT_EQ(test_case_multi_damage.test_get_damage_type().value() & code::client::messages::Damage::MAGIC,
                code::client::messages::Damage::MAGIC);
    EXPECT_EQ(test_case_multi_damage.test_get_damage_type().value() & code::client::messages::Damage::FIRE,
                code::client::messages::Damage::FIRE);
    EXPECT_TRUE(test_case_multi_damage.test_get_amount().has_value());
    EXPECT_FLOAT_EQ(test_case_multi_damage.test_get_amount().value(), 1.4f);

    code::client::messages::Damage test_output;
    test_case_multi_damage.apply_damage(test_output);
    EXPECT_EQ(test_output.damage_type() & code::client::messages::Damage::MAGIC,
                code::client::messages::Damage::MAGIC);
    EXPECT_EQ(test_output.damage_type() & code::client::messages::Damage::FIRE,
                code::client::messages::Damage::FIRE);
    EXPECT_FLOAT_EQ(test_output.amount(), 1.4f);
    
    test_case_multi_damage.apply_damage(test_output);
    EXPECT_EQ(test_output.damage_type() & code::client::messages::Damage::MAGIC,
                code::client::messages::Damage::MAGIC);
    EXPECT_EQ(test_output.damage_type() & code::client::messages::Damage::FIRE,
                code::client::messages::Damage::FIRE);
    EXPECT_FLOAT_EQ(test_output.amount(), 2.8f);

    code::client::messages::Damage test_output_two;
    test_output_two.set_damage_type(code::client::messages::Damage::PHYSICAL);
    test_output_two.set_amount(3.2f);
    test_case_multi_damage.apply_damage(test_output_two);
    EXPECT_EQ(test_output_two.damage_type() & code::client::messages::Damage::PHYSICAL,
                code::client::messages::Damage::PHYSICAL);
    EXPECT_EQ(test_output_two.damage_type() & code::client::messages::Damage::MAGIC,
                code::client::messages::Damage::MAGIC);
    EXPECT_EQ(test_output_two.damage_type() & code::client::messages::Damage::FIRE,
                code::client::messages::Damage::FIRE);
    EXPECT_FLOAT_EQ(test_output_two.amount(), 4.6f);
}

TEST(DamageTypeApply_Module, Test_DamageTypeApplyModule_Apply_NoDamage) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_no_damage = modules["TestData_NoDamage"];
    EXPECT_TRUE(test_no_damage);
    EXPECT_TRUE(test_no_damage.IsMap());
    DamageTypeApply_Module_Test test_case_no_damage("TestData_NoDamage");
    EXPECT_EQ(test_case_no_damage.get_module_name(), "TestData_NoDamage");
    EXPECT_FALSE(test_case_no_damage.init_module(test_no_damage));
    EXPECT_FALSE(test_case_no_damage.test_get_damage_type().has_value());
    EXPECT_FALSE(test_case_no_damage.test_get_amount().has_value());

    // Nothing happens
    code::client::messages::Damage test_output;
    test_case_no_damage.apply_damage(test_output);
    EXPECT_EQ(test_output.damage_type(), code::client::messages::Damage::NONE);
    EXPECT_FLOAT_EQ(test_output.amount(), 0.0f);

    code::client::messages::Damage test_output_two;
    test_output_two.set_damage_type(code::client::messages::Damage::PHYSICAL);
    test_output_two.set_amount(3.2f);
    test_case_no_damage.apply_damage(test_output_two);
    EXPECT_EQ(test_output_two.damage_type() & code::client::messages::Damage::PHYSICAL,
                code::client::messages::Damage::PHYSICAL);
    EXPECT_FLOAT_EQ(test_output_two.amount(), 3.2f);
}

TEST(DamageTypeApply_Module, Test_DamageTypeApplyModule_Apply_NoAmount) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_no_amount = modules["TestData_NoAmount"];
    EXPECT_TRUE(test_no_amount);
    EXPECT_TRUE(test_no_amount.IsMap());
    DamageTypeApply_Module_Test test_case_no_amount("TestData_NoAmount");
    EXPECT_EQ(test_case_no_amount.get_module_name(), "TestData_NoAmount");
    EXPECT_FALSE(test_case_no_amount.init_module(test_no_amount));
    EXPECT_FALSE(test_case_no_amount.test_get_damage_type().has_value());
    EXPECT_FALSE(test_case_no_amount.test_get_amount().has_value());

    // Nothing happens
    code::client::messages::Damage test_output;
    test_case_no_amount.apply_damage(test_output);
    EXPECT_EQ(test_output.damage_type(), code::client::messages::Damage::NONE);
    EXPECT_FLOAT_EQ(test_output.amount(), 0.0f);

    code::client::messages::Damage test_output_two;
    test_output_two.set_damage_type(code::client::messages::Damage::PHYSICAL);
    test_output_two.set_amount(3.2f);
    test_case_no_amount.apply_damage(test_output_two);
    EXPECT_EQ(test_output_two.damage_type() & code::client::messages::Damage::PHYSICAL,
                code::client::messages::Damage::PHYSICAL);
    EXPECT_FLOAT_EQ(test_output_two.amount(), 3.2f);
}

TEST(DamageTypeApply_Module, Test_DamageTypeApplyModule_Apply_NegativeAmount) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_negative_amount = modules["TestData_NegativeAmount"];
    EXPECT_TRUE(test_negative_amount);
    EXPECT_TRUE(test_negative_amount.IsMap());
    DamageTypeApply_Module_Test test_case_negative_amount("TestData_NegativeAmount");
    EXPECT_EQ(test_case_negative_amount.get_module_name(), "TestData_NegativeAmount");
    EXPECT_FALSE(test_case_negative_amount.init_module(test_negative_amount));
    EXPECT_FALSE(test_case_negative_amount.test_get_damage_type().has_value());
    EXPECT_FALSE(test_case_negative_amount.test_get_amount().has_value());

    // Nothing happens
    code::client::messages::Damage test_output;
    test_case_negative_amount.apply_damage(test_output);
    EXPECT_EQ(test_output.damage_type(), code::client::messages::Damage::NONE);
    EXPECT_FLOAT_EQ(test_output.amount(), 0.0f);

    code::client::messages::Damage test_output_two;
    test_output_two.set_damage_type(code::client::messages::Damage::PHYSICAL);
    test_output_two.set_amount(3.2f);
    test_case_negative_amount.apply_damage(test_output_two);
    EXPECT_EQ(test_output_two.damage_type() & code::client::messages::Damage::PHYSICAL,
                code::client::messages::Damage::PHYSICAL);
    EXPECT_FLOAT_EQ(test_output_two.amount(), 3.2f);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
