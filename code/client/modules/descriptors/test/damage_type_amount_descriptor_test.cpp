#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include "code/client/messages/proto/damage.pb.h"
#include "code/client/modules/descriptors/damage_type_amount_descriptor.h"

// This Test class is just being used to expose some data to the 
//  public for use in the Test functions. These are not meant to be available
//  in the normal api, so they are being wrapped in this class instead.
class DamageTypeAmount_Descriptor_Test : public Module::DamageTypeAmount_Descriptor {
public:
    DamageTypeAmount_Descriptor_Test() : DamageTypeAmount_Descriptor() {}

    std::optional<int32_t> test_get_damage_type() { return m_damage_type; }
    std::optional<float> test_get_amount() { return m_amount; }
    std::optional<float> test_get_partial_effect_amount() { return m_partial_effect_amount; }
};

std::string k_yaml_file = "code\\client\\modules\\descriptors\\test\\data\\test_damage_type_descriptor.yaml";

TEST(DamageTypeAmount_Descriptor, Test_DamageTypeAmountDescriptor_Simple_Apply) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_simple = modules["TestData_Simple"];
    EXPECT_TRUE(test_simple);
    EXPECT_TRUE(test_simple.IsMap());
    DamageTypeAmount_Descriptor_Test test_case_simple;
    EXPECT_TRUE(test_case_simple.init_descriptor(test_simple));
    EXPECT_TRUE(test_case_simple.test_get_damage_type().has_value());
    EXPECT_EQ(test_case_simple.test_get_damage_type().value() & code::client::messages::Damage::PHYSICAL,
                code::client::messages::Damage::PHYSICAL);
    EXPECT_TRUE(test_case_simple.test_get_amount().has_value());
    EXPECT_FLOAT_EQ(test_case_simple.test_get_amount().value(), 2.5f);
    EXPECT_FALSE(test_case_simple.test_get_partial_effect_amount().has_value());
}

TEST(DamageTypeAmount_Descriptor, Test_DamageTypeAmountDescriptor_SimplePartial_Apply) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());
    
    YAML::Node test_simple_partial = modules["TestData_Simple_Partial"];
    EXPECT_TRUE(test_simple_partial);
    EXPECT_TRUE(test_simple_partial.IsMap());
    DamageTypeAmount_Descriptor_Test test_case_simple_partial;
    EXPECT_TRUE(test_case_simple_partial.init_descriptor(test_simple_partial));
    EXPECT_TRUE(test_case_simple_partial.test_get_damage_type().has_value());
    EXPECT_EQ(test_case_simple_partial.test_get_damage_type().value() & code::client::messages::Damage::POISON,
                code::client::messages::Damage::POISON);
    EXPECT_TRUE(test_case_simple_partial.test_get_amount().has_value());
    EXPECT_FLOAT_EQ(test_case_simple_partial.test_get_amount().value(), 1.4f);
    EXPECT_TRUE(test_case_simple_partial.test_get_partial_effect_amount().has_value());
    EXPECT_FLOAT_EQ(test_case_simple_partial.test_get_partial_effect_amount().value(), 0.5f);
}

TEST(DamageTypeAmount_Descriptor, Test_DamageTypeAmountDescriptor_MultipleDamage_Apply) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_multi_damage = modules["TestData_MultipleDamage"];
    EXPECT_TRUE(test_multi_damage);
    EXPECT_TRUE(test_multi_damage.IsMap());
    DamageTypeAmount_Descriptor_Test test_case_multi_damage;
    EXPECT_TRUE(test_case_multi_damage.init_descriptor(test_multi_damage));
    EXPECT_TRUE(test_case_multi_damage.test_get_damage_type().has_value());
    EXPECT_EQ(test_case_multi_damage.test_get_damage_type().value() & code::client::messages::Damage::MAGIC,
                code::client::messages::Damage::MAGIC);
    EXPECT_EQ(test_case_multi_damage.test_get_damage_type().value() & code::client::messages::Damage::FIRE,
                code::client::messages::Damage::FIRE);
    EXPECT_TRUE(test_case_multi_damage.test_get_amount().has_value());
    EXPECT_FLOAT_EQ(test_case_multi_damage.test_get_amount().value(), 1.4f);
    EXPECT_TRUE(test_case_multi_damage.test_get_partial_effect_amount().has_value());
    EXPECT_FLOAT_EQ(test_case_multi_damage.test_get_partial_effect_amount().value(), 0.5f);
}

TEST(DamageTypeAmount_Descriptor, Test_DamageTypeAmountDescriptor_MultipleDamage_BadPartial_Apply) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_multi_damage_partial = modules["TestData_MultipleDamage_BadPartial"];
    EXPECT_TRUE(test_multi_damage_partial);
    EXPECT_TRUE(test_multi_damage_partial.IsMap());
    DamageTypeAmount_Descriptor_Test test_case_multi_damage_partial;
    EXPECT_FALSE(test_case_multi_damage_partial.init_descriptor(test_multi_damage_partial));
    EXPECT_FALSE(test_case_multi_damage_partial.test_get_damage_type().has_value());
    EXPECT_FALSE(test_case_multi_damage_partial.test_get_amount().has_value());
    EXPECT_FALSE(test_case_multi_damage_partial.test_get_partial_effect_amount().has_value());
}

TEST(DamageTypeAmount_Descriptor, Test_DamageTypeAmountDescriptor_NoDamage_Apply) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_no_damage = modules["TestData_NoDamage"];
    EXPECT_TRUE(test_no_damage);
    EXPECT_TRUE(test_no_damage.IsMap());
    DamageTypeAmount_Descriptor_Test test_case_no_damage;
    EXPECT_FALSE(test_case_no_damage.init_descriptor(test_no_damage));
    EXPECT_FALSE(test_case_no_damage.test_get_damage_type().has_value());
    EXPECT_FALSE(test_case_no_damage.test_get_amount().has_value());
    EXPECT_FALSE(test_case_no_damage.test_get_partial_effect_amount().has_value());
}

TEST(DamageTypeAmount_Descriptor, Test_DamageTypeAmountDescriptor_No_Amount_Apply) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_no_amount = modules["TestData_NoAmount"];
    EXPECT_TRUE(test_no_amount);
    EXPECT_TRUE(test_no_amount.IsMap());
    DamageTypeAmount_Descriptor_Test test_case_no_amount;
    EXPECT_FALSE(test_case_no_amount.init_descriptor(test_no_amount));
    EXPECT_FALSE(test_case_no_amount.test_get_damage_type().has_value());
    EXPECT_FALSE(test_case_no_amount.test_get_amount().has_value());
    EXPECT_FALSE(test_case_no_amount.test_get_partial_effect_amount().has_value());
}

TEST(DamageTypeAmount_Descriptor, Test_DamageTypeAmountDescriptor_Negative_Amount_Apply) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_negative_amount = modules["TestData_NegativeAmount"];
    EXPECT_TRUE(test_negative_amount);
    EXPECT_TRUE(test_negative_amount.IsMap());
    DamageTypeAmount_Descriptor_Test test_case_negative_amount;
    EXPECT_FALSE(test_case_negative_amount.init_descriptor(test_negative_amount));
    EXPECT_FALSE(test_case_negative_amount.test_get_damage_type().has_value());
    EXPECT_FALSE(test_case_negative_amount.test_get_amount().has_value());
    EXPECT_FALSE(test_case_negative_amount.test_get_partial_effect_amount().has_value());
}

TEST(DamageTypeAmount_Descriptor, Test_DamageTypeAmountDescriptor_PartialLargerThanAmount_Apply) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_partial_larger_than_amount = modules["TestData_PartialLargerThanAmount"];
    EXPECT_TRUE(test_partial_larger_than_amount);
    EXPECT_TRUE(test_partial_larger_than_amount.IsMap());
    DamageTypeAmount_Descriptor_Test test_case_partial_larger_than_amount;
    EXPECT_FALSE(test_case_partial_larger_than_amount.init_descriptor(test_partial_larger_than_amount));
    EXPECT_FALSE(test_case_partial_larger_than_amount.test_get_damage_type().has_value());
    EXPECT_FALSE(test_case_partial_larger_than_amount.test_get_amount().has_value());
    EXPECT_FALSE(test_case_partial_larger_than_amount.test_get_partial_effect_amount().has_value());
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
