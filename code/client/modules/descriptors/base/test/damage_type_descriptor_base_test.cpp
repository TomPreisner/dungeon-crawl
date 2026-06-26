#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include "code/client/messages/proto/damage.pb.h"
#include "code/client/modules/descriptors/base/damage_type_descriptor_base.h"

// This Test class is just being used to expose some data to the 
//  public for use in the Test functions. These are not meant to be available
//  in the normal api, so they are being wrapped in this class instead.
class DamageTypeDescriptor_Base_Test : public Module::DamageTypeDescriptor_Base {
public:
    DamageTypeDescriptor_Base_Test() : DamageTypeDescriptor_Base() {}

    std::optional<int32_t> test_get_damage_type() { return m_damage_type; }
    std::optional<float> test_get_amount() { return m_amount; }
};

std::string k_yaml_file = "code\\client\\modules\\descriptors\\base\\test\\data\\damage_type.yaml";

TEST(DamageTypeDescriptor_Base, Test_DamageTypeDescriptorCreation_Simple) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_simple = modules["TestData_Simple"];
    EXPECT_TRUE(test_simple);
    EXPECT_TRUE(test_simple.IsMap());
    DamageTypeDescriptor_Base_Test test_case_simple;
    EXPECT_TRUE(test_case_simple.init_module(test_simple));
    EXPECT_TRUE(test_case_simple.test_get_damage_type().has_value());
    EXPECT_EQ(test_case_simple.test_get_damage_type().value() & code::client::messages::Damage::PHYSICAL,
                code::client::messages::Damage::PHYSICAL);
    EXPECT_TRUE(test_case_simple.test_get_amount().has_value());
    EXPECT_FLOAT_EQ(test_case_simple.test_get_amount().value(), 2.5f);
}

TEST(DamageTypeDescriptor_Base, Test_DamageTypeDescriptorCreation_MultipleDamage) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_multi_damage = modules["TestData_MultipleDamage"];
    EXPECT_TRUE(test_multi_damage);
    EXPECT_TRUE(test_multi_damage.IsMap());
    DamageTypeDescriptor_Base_Test test_case_multi_damage;
    EXPECT_TRUE(test_case_multi_damage.init_module(test_multi_damage));
    EXPECT_TRUE(test_case_multi_damage.test_get_damage_type().has_value());
    EXPECT_EQ(test_case_multi_damage.test_get_damage_type().value() & code::client::messages::Damage::MAGIC,
                code::client::messages::Damage::MAGIC);
    EXPECT_EQ(test_case_multi_damage.test_get_damage_type().value() & code::client::messages::Damage::FIRE,
                code::client::messages::Damage::FIRE);
    EXPECT_TRUE(test_case_multi_damage.test_get_amount().has_value());
    EXPECT_FLOAT_EQ(test_case_multi_damage.test_get_amount().value(), 1.4f);
}

TEST(DamageTypeDescriptor_Base, Test_DamageTypeDescriptorCreation_NoDamage) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_no_damage = modules["TestData_NoDamage"];
    EXPECT_TRUE(test_no_damage);
    EXPECT_TRUE(test_no_damage.IsMap());
    DamageTypeDescriptor_Base_Test test_case_no_damage;
    EXPECT_FALSE(test_case_no_damage.init_module(test_no_damage));
    EXPECT_FALSE(test_case_no_damage.test_get_damage_type().has_value());
    EXPECT_FALSE(test_case_no_damage.test_get_amount().has_value());
}

TEST(DamageTypeDescriptor_Base, Test_DamageTypeDescriptorCreation_DamageNotSequence) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_damage_not_seq = modules["TestData_DamageNotSequence"];
    EXPECT_TRUE(test_damage_not_seq);
    EXPECT_TRUE(test_damage_not_seq.IsMap());
    DamageTypeDescriptor_Base_Test test_case_damage_not_seq;
    EXPECT_FALSE(test_case_damage_not_seq.init_module(test_damage_not_seq));
    EXPECT_FALSE(test_case_damage_not_seq.test_get_damage_type().has_value());
    EXPECT_FALSE(test_case_damage_not_seq.test_get_amount().has_value());
}

TEST(DamageTypeDescriptor_Base, Test_DamageTypeDescriptorCreation_DamageInvalidType) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_damage_invalid = modules["TestData_DamageInvalidType"];
    EXPECT_TRUE(test_damage_invalid);
    EXPECT_TRUE(test_damage_invalid.IsMap());
    DamageTypeDescriptor_Base_Test test_case_damage_invalid;
    EXPECT_FALSE(test_case_damage_invalid.init_module(test_damage_invalid));
    EXPECT_FALSE(test_case_damage_invalid.test_get_damage_type().has_value());
    EXPECT_FALSE(test_case_damage_invalid.test_get_amount().has_value());
}

TEST(DamageTypeDescriptor_Base, Test_DamageTypeDescriptorCreation_NoAmount) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_no_amount = modules["TestData_NoAmount"];
    EXPECT_TRUE(test_no_amount);
    EXPECT_TRUE(test_no_amount.IsMap());
    DamageTypeDescriptor_Base_Test test_case_no_amount;
    EXPECT_FALSE(test_case_no_amount.init_module(test_no_amount));
    EXPECT_FALSE(test_case_no_amount.test_get_damage_type().has_value());
    EXPECT_FALSE(test_case_no_amount.test_get_amount().has_value());
}

TEST(DamageTypeDescriptor_Base, Test_DamageTypeDescriptorCreation_AmountNotFloat) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_amount_not_float = modules["TestData_AmountNotFloat"];
    EXPECT_TRUE(test_amount_not_float);
    EXPECT_TRUE(test_amount_not_float.IsMap());
    DamageTypeDescriptor_Base_Test test_case_amount_not_float;
    EXPECT_FALSE(test_case_amount_not_float.init_module(test_amount_not_float));
    EXPECT_FALSE(test_case_amount_not_float.test_get_damage_type().has_value());
    EXPECT_FALSE(test_case_amount_not_float.test_get_amount().has_value());
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
