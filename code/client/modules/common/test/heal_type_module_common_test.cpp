#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include "code/client/health/proto/heal.pb.h"
#include "code/client/modules/common/heal_type_module_common.h"

// This Test class is just being used to expose some data to the 
//  public for use in the Test functions. These are not meant to be available
//  in the normal api, so they are being wrapped in this class instead.
class HealTypeModule_Common_Test : public Module::HealTypeModule_Common {
public:
    explicit HealTypeModule_Common_Test(const std::string& name) : HealTypeModule_Common(name) {}
    
    std::optional<Heal::HealType> test_get_heal_type() { return m_heal_type; }
    std::optional<float> test_get_amount() { return m_amount; }
};

std::string k_yaml_file = "code\\client\\modules\\common\\test\\data\\test_heal_type_module_common.yaml";

TEST(HealTypeModule_Common, Test_HealTypeModuleCreation) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_simple = modules["TestData_Simple"];
    EXPECT_TRUE(test_simple);
    EXPECT_TRUE(test_simple.IsMap());
    HealTypeModule_Common_Test test_case_simple("TestData_Simple");
    EXPECT_EQ(test_case_simple.get_module_name(), "TestData_Simple");
    EXPECT_TRUE(test_case_simple.init_module(test_simple));
    EXPECT_TRUE(test_case_simple.test_get_heal_type().has_value());
    EXPECT_EQ(test_case_simple.test_get_heal_type().value(), Heal::POTION);
    EXPECT_TRUE(test_case_simple.test_get_amount().has_value());
    EXPECT_FLOAT_EQ(test_case_simple.test_get_amount().value(), 2.5f);

    YAML::Node test_no_heal = modules["TestData_NoHeal"];
    EXPECT_TRUE(test_no_heal);
    EXPECT_TRUE(test_no_heal.IsMap());
    HealTypeModule_Common_Test test_case_no_heal("TestData_NoHeal");
    EXPECT_EQ(test_case_no_heal.get_module_name(), "TestData_NoHeal");
    EXPECT_FALSE(test_case_no_heal.init_module(test_no_heal));
    EXPECT_FALSE(test_case_no_heal.test_get_heal_type().has_value());
    EXPECT_FALSE(test_case_no_heal.test_get_amount().has_value());

    YAML::Node test_heal_not_string = modules["TestData_HealNotString"];
    EXPECT_TRUE(test_heal_not_string);
    EXPECT_TRUE(test_heal_not_string.IsMap());
    HealTypeModule_Common_Test test_case_heal_not_string("TestData_HealNotString");
    EXPECT_EQ(test_case_heal_not_string.get_module_name(), "TestData_HealNotString");
    EXPECT_FALSE(test_case_heal_not_string.init_module(test_heal_not_string));
    EXPECT_FALSE(test_case_heal_not_string.test_get_heal_type().has_value());
    EXPECT_FALSE(test_case_heal_not_string.test_get_amount().has_value());

    YAML::Node test_heal_invalid = modules["TestData_HealInvalidType"];
    EXPECT_TRUE(test_heal_invalid);
    EXPECT_TRUE(test_heal_invalid.IsMap());
    HealTypeModule_Common_Test test_case_heal_invalid("TestData_HealInvalidType");
    EXPECT_EQ(test_case_heal_invalid.get_module_name(), "TestData_HealInvalidType");
    EXPECT_FALSE(test_case_heal_invalid.init_module(test_heal_invalid));
    EXPECT_FALSE(test_case_heal_invalid.test_get_heal_type().has_value());
    EXPECT_FALSE(test_case_heal_invalid.test_get_amount().has_value());

    YAML::Node test_no_amount = modules["TestData_NoAmount"];
    EXPECT_TRUE(test_no_amount);
    EXPECT_TRUE(test_no_amount.IsMap());
    HealTypeModule_Common_Test test_case_no_amount("TestData_NoAmount");
    EXPECT_EQ(test_case_no_amount.get_module_name(), "TestData_NoAmount");
    EXPECT_FALSE(test_case_no_amount.init_module(test_no_amount));
    EXPECT_FALSE(test_case_no_amount.test_get_heal_type().has_value());
    EXPECT_FALSE(test_case_no_amount.test_get_amount().has_value());

    YAML::Node test_amount_not_float = modules["TestData_AmountNotFloat"];
    EXPECT_TRUE(test_amount_not_float);
    EXPECT_TRUE(test_amount_not_float.IsMap());
    HealTypeModule_Common_Test test_case_amount_not_float("TestData_AmountNotFloat");
    EXPECT_EQ(test_case_amount_not_float.get_module_name(), "TestData_AmountNotFloat");
    EXPECT_FALSE(test_case_amount_not_float.init_module(test_amount_not_float));
    EXPECT_FALSE(test_case_amount_not_float.test_get_heal_type().has_value());
    EXPECT_FALSE(test_case_amount_not_float.test_get_amount().has_value());
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
