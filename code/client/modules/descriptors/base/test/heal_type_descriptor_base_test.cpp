#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include "code/client/messages/proto/heal.pb.h"
#include "code/client/modules/descriptors/base/heal_type_descriptor_base.h"

// This Test class is just being used to expose some data to the 
//  public for use in the Test functions. These are not meant to be available
//  in the normal api, so they are being wrapped in this class instead.
class HealTypeDescriptor_Base_Test : public Module::HealTypeDescriptor_Base {
public:
    HealTypeDescriptor_Base_Test() : HealTypeDescriptor_Base() {}

    std::optional<code::client::messages::Heal::HealType> test_get_heal_type() { return m_heal_type; }
    std::optional<float> test_get_amount() { return m_amount; }
};

std::string k_yaml_file = "code\\client\\modules\\descriptors\\base\\test\\data\\heal_type.yaml";

TEST(HealTypeDescriptor_Base, Test_HealTypeDescriptorCreation_Simple) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_simple = modules["TestData_Simple"];
    EXPECT_TRUE(test_simple);
    EXPECT_TRUE(test_simple.IsMap());
    HealTypeDescriptor_Base_Test test_case_simple;
    EXPECT_TRUE(test_case_simple.init_descriptor(test_simple));
    EXPECT_TRUE(test_case_simple.test_get_heal_type().has_value());
    EXPECT_EQ(test_case_simple.test_get_heal_type().value(), code::client::messages::Heal::POTION);
    EXPECT_TRUE(test_case_simple.test_get_amount().has_value());
    EXPECT_FLOAT_EQ(test_case_simple.test_get_amount().value(), 2.5f);
}

TEST(HealTypeDescriptor_Base, Test_HealTypeDescriptorCreation_NoHeal) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_no_heal = modules["TestData_NoHeal"];
    EXPECT_TRUE(test_no_heal);
    EXPECT_TRUE(test_no_heal.IsMap());
    HealTypeDescriptor_Base_Test test_case_no_heal;
    EXPECT_FALSE(test_case_no_heal.init_descriptor(test_no_heal));
    EXPECT_FALSE(test_case_no_heal.test_get_heal_type().has_value());
    EXPECT_FALSE(test_case_no_heal.test_get_amount().has_value());
}

TEST(HealTypeDescriptor_Base, Test_HealTypeDescriptorCreation_HealNotString) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_heal_not_string = modules["TestData_HealNotString"];
    EXPECT_TRUE(test_heal_not_string);
    EXPECT_TRUE(test_heal_not_string.IsMap());
    HealTypeDescriptor_Base_Test test_case_heal_not_string;
    EXPECT_FALSE(test_case_heal_not_string.init_descriptor(test_heal_not_string));
    EXPECT_FALSE(test_case_heal_not_string.test_get_heal_type().has_value());
    EXPECT_FALSE(test_case_heal_not_string.test_get_amount().has_value());
}

TEST(HealTypeDescriptor_Base, Test_HealTypeDescriptorCreation_HealInvalidType) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_heal_invalid = modules["TestData_HealInvalidType"];
    EXPECT_TRUE(test_heal_invalid);
    EXPECT_TRUE(test_heal_invalid.IsMap());
    HealTypeDescriptor_Base_Test test_case_heal_invalid;
    EXPECT_FALSE(test_case_heal_invalid.init_descriptor(test_heal_invalid));
    EXPECT_FALSE(test_case_heal_invalid.test_get_heal_type().has_value());
    EXPECT_FALSE(test_case_heal_invalid.test_get_amount().has_value());
}

TEST(HealTypeDescriptor_Base, Test_HealTypeDescriptorCreation_NoAmount) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_no_amount = modules["TestData_NoAmount"];
    EXPECT_TRUE(test_no_amount);
    EXPECT_TRUE(test_no_amount.IsMap());
    HealTypeDescriptor_Base_Test test_case_no_amount;
    EXPECT_FALSE(test_case_no_amount.init_descriptor(test_no_amount));
    EXPECT_FALSE(test_case_no_amount.test_get_heal_type().has_value());
    EXPECT_FALSE(test_case_no_amount.test_get_amount().has_value());
}

TEST(HealTypeDescriptor_Base, Test_HealTypeDescriptorCreation_AmountNotFloat) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_amount_not_float = modules["TestData_AmountNotFloat"];
    EXPECT_TRUE(test_amount_not_float);
    EXPECT_TRUE(test_amount_not_float.IsMap());
    HealTypeDescriptor_Base_Test test_case_amount_not_float;
    EXPECT_FALSE(test_case_amount_not_float.init_descriptor(test_amount_not_float));
    EXPECT_FALSE(test_case_amount_not_float.test_get_heal_type().has_value());
    EXPECT_FALSE(test_case_amount_not_float.test_get_amount().has_value());
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
