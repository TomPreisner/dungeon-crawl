#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include "code/client/modules/descriptors/heal_type_amount_descriptor.h"

// This Test class is just being used to expose some data to the 
//  public for use in the Test functions. These are not meant to be available
//  in the normal api, so they are being wrapped in this class instead.
class HealTypeAmount_Descriptor_Test : public Module::HealTypeAmount_Descriptor {
public:
    HealTypeAmount_Descriptor_Test() : HealTypeAmount_Descriptor() {}

    std::optional<int32_t> test_get_heal_type() { return m_heal_type; }
    std::optional<float> test_get_amount() { return m_amount; }
};

std::string k_yaml_file = "code\\client\\modules\\descriptors\\test\\data\\test_heal_type_descriptor.yaml";

TEST(HealTypeAmount_Descriptor, Test_HealTypeAmountDescriptor_Simple_Apply) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_simple = modules["TestData_Simple"];
    EXPECT_TRUE(test_simple);
    EXPECT_TRUE(test_simple.IsMap());
    HealTypeAmount_Descriptor_Test test_case_simple;
    EXPECT_TRUE(test_case_simple.init_descriptor(test_simple));
    EXPECT_TRUE(test_case_simple.test_get_heal_type().has_value());
    EXPECT_EQ(test_case_simple.test_get_heal_type().value(), code::client::messages::Heal::POTION);
    EXPECT_TRUE(test_case_simple.test_get_amount().has_value());
    EXPECT_FLOAT_EQ(test_case_simple.test_get_amount().value(), 2.5f);
}

TEST(HealTypeAmount_Descriptor, Test_HealTypeAmountDescriptor_NoHeal_Apply) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_no_heal = modules["TestData_NoHeal"];
    EXPECT_TRUE(test_no_heal);
    EXPECT_TRUE(test_no_heal.IsMap());
    HealTypeAmount_Descriptor_Test test_case_no_heal;
    EXPECT_FALSE(test_case_no_heal.init_descriptor(test_no_heal));
    EXPECT_FALSE(test_case_no_heal.test_get_heal_type().has_value());
    EXPECT_FALSE(test_case_no_heal.test_get_amount().has_value());
}

TEST(HealTypeAmount_Descriptor, Test_HealTypeAmountDescriptor_HealNotString_Apply) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_heal_not_string = modules["TestData_HealString"];
    EXPECT_TRUE(test_heal_not_string);
    EXPECT_TRUE(test_heal_not_string.IsMap());
    HealTypeAmount_Descriptor_Test test_case_heal_not_string;
    EXPECT_FALSE(test_case_heal_not_string.init_descriptor(test_heal_not_string));
    EXPECT_FALSE(test_case_heal_not_string.test_get_heal_type().has_value());
    EXPECT_FALSE(test_case_heal_not_string.test_get_amount().has_value());
}

TEST(HealTypeAmount_Descriptor, Test_HealTypeAmountDescriptor_HealInvalidType_Apply) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_heal_invalid_type = modules["TestData_HealInvalidType"];
    EXPECT_TRUE(test_heal_invalid_type);
    EXPECT_TRUE(test_heal_invalid_type.IsMap());
    HealTypeAmount_Descriptor_Test test_case_heal_invalid_type;
    EXPECT_FALSE(test_case_heal_invalid_type.init_descriptor(test_heal_invalid_type));
    EXPECT_FALSE(test_case_heal_invalid_type.test_get_heal_type().has_value());
    EXPECT_FALSE(test_case_heal_invalid_type.test_get_amount().has_value());
}

TEST(HealTypeAmount_Descriptor, Test_HealTypeAmountDescriptor_No_Amount_Apply) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_no_amount = modules["TestData_NoAmount"];
    EXPECT_TRUE(test_no_amount);
    EXPECT_TRUE(test_no_amount.IsMap());
    HealTypeAmount_Descriptor_Test test_case_no_amount;
    EXPECT_FALSE(test_case_no_amount.init_descriptor(test_no_amount));
    EXPECT_FALSE(test_case_no_amount.test_get_heal_type().has_value());
    EXPECT_FALSE(test_case_no_amount.test_get_amount().has_value());
}

TEST(HealTypeAmount_Descriptor, Test_HealTypeAmountDescriptor_Negative_Amount_Apply) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_negative_amount = modules["TestData_NegativeAmount"];
    EXPECT_TRUE(test_negative_amount);
    EXPECT_TRUE(test_negative_amount.IsMap());
    HealTypeAmount_Descriptor_Test test_case_negative_amount;
    EXPECT_FALSE(test_case_negative_amount.init_descriptor(test_negative_amount));
    EXPECT_FALSE(test_case_negative_amount.test_get_heal_type().has_value());
    EXPECT_FALSE(test_case_negative_amount.test_get_amount().has_value());
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
