#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include "code/client/messages/proto/damage.pb.h"
#include "code/client/modules/descriptors/damage_type_percent_descriptor.h"

// This Test class is just being used to expose some data to the 
//  public for use in the Test functions. These are not meant to be available
//  in the normal api, so they are being wrapped in this class instead.
class DamageTypePercent_Descriptor_Test : public Module::DamageTypePercent_Descriptor {
public:
    DamageTypePercent_Descriptor_Test() : DamageTypePercent_Descriptor() {}
    
    std::optional<int32_t> test_get_damage_type() { return m_damage_type; }
    std::optional<float> test_get_amount() { return m_amount; }
    std::optional<float> test_get_partial_effect_amount() { return m_partial_effect_amount; }
};

std::string k_yaml_file = "code\\client\\modules\\descriptors\\test\\data\\test_damage_type_descriptor.yaml";

TEST(DamageTypePercent_Descriptor, Test_DamageTypePercentDescriptor_Simple_Apply) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_simple = modules["TestData_SimplePercent"];
    EXPECT_TRUE(test_simple);
    EXPECT_TRUE(test_simple.IsMap());
    DamageTypePercent_Descriptor_Test test_case_simple;
    EXPECT_TRUE(test_case_simple.init_module(test_simple));
    EXPECT_TRUE(test_case_simple.test_get_damage_type().has_value());
    EXPECT_EQ(test_case_simple.test_get_damage_type().value() & code::client::messages::Damage::PHYSICAL,
                code::client::messages::Damage::PHYSICAL);
    EXPECT_TRUE(test_case_simple.test_get_amount().has_value());
    EXPECT_FLOAT_EQ(test_case_simple.test_get_amount().value(), 0.8f);
    EXPECT_FALSE(test_case_simple.test_get_partial_effect_amount().has_value());
}

TEST(DamageTypePercent_Descriptor, Test_DamageTypePercentDescriptor_SimplePartial_Apply) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());
    
    YAML::Node test_simple_partial = modules["TestData_SimplePercent_Partial"];
    EXPECT_TRUE(test_simple_partial);
    EXPECT_TRUE(test_simple_partial.IsMap());
    DamageTypePercent_Descriptor_Test test_case_simple_partial;
    EXPECT_TRUE(test_case_simple_partial.init_module(test_simple_partial));
    EXPECT_TRUE(test_case_simple_partial.test_get_damage_type().has_value());
    EXPECT_EQ(test_case_simple_partial.test_get_damage_type().value() & code::client::messages::Damage::POISON,
                code::client::messages::Damage::POISON);
    EXPECT_TRUE(test_case_simple_partial.test_get_amount().has_value());
    EXPECT_FLOAT_EQ(test_case_simple_partial.test_get_amount().value(), 0.8f);
    EXPECT_TRUE(test_case_simple_partial.test_get_partial_effect_amount().has_value());
    EXPECT_FLOAT_EQ(test_case_simple_partial.test_get_partial_effect_amount().value(), 0.4f);
}

TEST(DamageTypePercent_Descriptor, Test_DamageTypePercentDescriptor_MultipleDamage_Apply) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_multi_damage = modules["TestData_MultipleDamagePercent"];
    EXPECT_TRUE(test_multi_damage);
    EXPECT_TRUE(test_multi_damage.IsMap());
    DamageTypePercent_Descriptor_Test test_case_multi_damage;
    EXPECT_TRUE(test_case_multi_damage.init_module(test_multi_damage));
    EXPECT_TRUE(test_case_multi_damage.test_get_damage_type().has_value());
    EXPECT_EQ(test_case_multi_damage.test_get_damage_type().value() & code::client::messages::Damage::MAGIC,
                code::client::messages::Damage::MAGIC);
    EXPECT_EQ(test_case_multi_damage.test_get_damage_type().value() & code::client::messages::Damage::FIRE,
                code::client::messages::Damage::FIRE);
    EXPECT_TRUE(test_case_multi_damage.test_get_amount().has_value());
    EXPECT_FLOAT_EQ(test_case_multi_damage.test_get_amount().value(), 0.8f);
    EXPECT_TRUE(test_case_multi_damage.test_get_partial_effect_amount().has_value());
    EXPECT_FLOAT_EQ(test_case_multi_damage.test_get_partial_effect_amount().value(), 0.3f);
}

TEST(DamageTypePercent_Descriptor, Test_DamageTypePercentDescriptor_MultipleDamage_BadPartial_Apply) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());
    
    YAML::Node test_multi_damage_partial = modules["TestData_MultipleDamagePercent_BadPartial"];
    EXPECT_TRUE(test_multi_damage_partial);
    EXPECT_TRUE(test_multi_damage_partial.IsMap());
    DamageTypePercent_Descriptor_Test test_case_multi_damage_partial;
    EXPECT_FALSE(test_case_multi_damage_partial.init_module(test_multi_damage_partial));
    EXPECT_FALSE(test_case_multi_damage_partial.test_get_damage_type().has_value());
    EXPECT_FALSE(test_case_multi_damage_partial.test_get_amount().has_value());
    EXPECT_FALSE(test_case_multi_damage_partial.test_get_partial_effect_amount().has_value());
}

TEST(DamageTypePercent_Descriptor, Test_DamageTypePercentDescriptor_NoDamage_Apply) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_no_damage = modules["TestData_NoDamage"];
    EXPECT_TRUE(test_no_damage);
    EXPECT_TRUE(test_no_damage.IsMap());
    DamageTypePercent_Descriptor_Test test_case_no_damage;
    EXPECT_FALSE(test_case_no_damage.init_module(test_no_damage));
    EXPECT_FALSE(test_case_no_damage.test_get_damage_type().has_value());
    EXPECT_FALSE(test_case_no_damage.test_get_amount().has_value());
    EXPECT_FALSE(test_case_no_damage.test_get_partial_effect_amount().has_value());
}

TEST(DamageTypePercent_Descriptor, Test_DamageTypePercentDescriptor_NoAmount_Apply) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_no_amount = modules["TestData_NoAmount"];
    EXPECT_TRUE(test_no_amount);
    EXPECT_TRUE(test_no_amount.IsMap());
    DamageTypePercent_Descriptor_Test test_case_no_amount;
    EXPECT_FALSE(test_case_no_amount.init_module(test_no_amount));
    EXPECT_FALSE(test_case_no_amount.test_get_damage_type().has_value());
    EXPECT_FALSE(test_case_no_amount.test_get_amount().has_value());
    EXPECT_FALSE(test_case_no_amount.test_get_partial_effect_amount().has_value());
}

TEST(DamageTypePercent_Descriptor, Test_DamageTypePercentDescriptor_NegativeAmount_Apply) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_negative_amount = modules["TestData_NegativeAmount"];
    EXPECT_TRUE(test_negative_amount);
    EXPECT_TRUE(test_negative_amount.IsMap());
    DamageTypePercent_Descriptor_Test test_case_negative_amount;
    EXPECT_FALSE(test_case_negative_amount.init_module(test_negative_amount));
    EXPECT_FALSE(test_case_negative_amount.test_get_damage_type().has_value());
    EXPECT_FALSE(test_case_negative_amount.test_get_amount().has_value());
    EXPECT_FALSE(test_case_negative_amount.test_get_partial_effect_amount().has_value());
}

TEST(DamageTypePercent_Descriptor, Test_DamageTypePercentDescriptor_PercentTooLarge_Apply) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_percent_too_large = modules["TestData_PercentTooLarge"];
    EXPECT_TRUE(test_percent_too_large);
    EXPECT_TRUE(test_percent_too_large.IsMap());
    DamageTypePercent_Descriptor_Test test_case_percent_too_large;
    EXPECT_FALSE(test_case_percent_too_large.init_module(test_percent_too_large));
    EXPECT_FALSE(test_case_percent_too_large.test_get_damage_type().has_value());
    EXPECT_FALSE(test_case_percent_too_large.test_get_amount().has_value());
    EXPECT_FALSE(test_case_percent_too_large.test_get_partial_effect_amount().has_value());
}

TEST(DamageTypePercent_Descriptor, Test_DamageTypePercentDescriptor_PercentPartialNegative_Apply) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_percent_partial_negative = modules["TestData_PercentPartialNegative"];
    EXPECT_TRUE(test_percent_partial_negative);
    EXPECT_TRUE(test_percent_partial_negative.IsMap());
    DamageTypePercent_Descriptor_Test test_case_percent_partial_negative;
    EXPECT_FALSE(test_case_percent_partial_negative.init_module(test_percent_partial_negative));
    EXPECT_FALSE(test_case_percent_partial_negative.test_get_damage_type().has_value());
    EXPECT_FALSE(test_case_percent_partial_negative.test_get_amount().has_value());
    EXPECT_FALSE(test_case_percent_partial_negative.test_get_partial_effect_amount().has_value());
}

TEST(DamageTypePercent_Descriptor, Test_DamageTypePercentDescriptor_PercentPartialLargerThanAmount_Apply) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_percent_partial_larger_than_amount = modules["TestData_PercentPartialLargerThanAmount"];
    EXPECT_TRUE(test_percent_partial_larger_than_amount);
    EXPECT_TRUE(test_percent_partial_larger_than_amount.IsMap());
    DamageTypePercent_Descriptor_Test test_case_percent_partial_larger_than_amount;
    EXPECT_FALSE(test_case_percent_partial_larger_than_amount.init_module(test_percent_partial_larger_than_amount));
    EXPECT_FALSE(test_case_percent_partial_larger_than_amount.test_get_damage_type().has_value());
    EXPECT_FALSE(test_case_percent_partial_larger_than_amount.test_get_amount().has_value());
    EXPECT_FALSE(test_case_percent_partial_larger_than_amount.test_get_partial_effect_amount().has_value());
}

TEST(DamageTypePercent_Descriptor, Test_DamageTypePercentDescriptor_PercentPartialTooLarge_Apply) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_percent_partial_too_large = modules["TestData_PercentPartialTooLarge"];
    EXPECT_TRUE(test_percent_partial_too_large);
    EXPECT_TRUE(test_percent_partial_too_large.IsMap());
    DamageTypePercent_Descriptor_Test test_case_percent_partial_too_large;
    EXPECT_FALSE(test_case_percent_partial_too_large.init_module(test_percent_partial_too_large));
    EXPECT_FALSE(test_case_percent_partial_too_large.test_get_damage_type().has_value());
    EXPECT_FALSE(test_case_percent_partial_too_large.test_get_amount().has_value());
    EXPECT_FALSE(test_case_percent_partial_too_large.test_get_partial_effect_amount().has_value());
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
