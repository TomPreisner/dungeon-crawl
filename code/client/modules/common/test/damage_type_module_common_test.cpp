#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include "code/client/damage/proto/damage.pb.h"
#include "code/client/modules/common/damage_type_module_common.h"

// This Test class is just being used to expose some data to the 
//  public for use in the Test functions. These are not meant to be available
//  in the normal api, so they are being wrapped in this class instead.
class DamageTypeModule_Common_Test : public Module::DamageTypeModule_Common {
public:
    explicit DamageTypeModule_Common_Test(const std::string& name) : DamageTypeModule_Common(name) {}
    
    std::optional<int32_t> test_get_damage_type() { return m_damage_type; }
    std::optional<float> test_get_amount() { return m_amount; }
};

std::string k_yaml_file = "code\\client\\modules\\common\\test\\data\\test_damage_type_module_common.yaml";

TEST(DamageTypeModule_Common, Test_DamageTypeModuleCreation) {
    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_simple = modules["TestData_Simple"];
    EXPECT_TRUE(test_simple);
    EXPECT_TRUE(test_simple.IsMap());
    DamageTypeModule_Common_Test test_case_simple("TestData_Simple");
    EXPECT_EQ(test_case_simple.get_module_name(), "TestData_Simple");
    EXPECT_TRUE(test_case_simple.init_module(test_simple));
    EXPECT_TRUE(test_case_simple.test_get_damage_type().has_value());
    EXPECT_EQ(test_case_simple.test_get_damage_type().value() & Damage::PHYSICAL, Damage::PHYSICAL);
    EXPECT_TRUE(test_case_simple.test_get_amount().has_value());
    EXPECT_FLOAT_EQ(test_case_simple.test_get_amount().value(), 2.5f);

    YAML::Node test_multi_damage = modules["TestData_MultipleDamage"];
    EXPECT_TRUE(test_multi_damage);
    EXPECT_TRUE(test_multi_damage.IsMap());
    DamageTypeModule_Common_Test test_case_multi_damage("TestData_MultipleDamage");
    EXPECT_EQ(test_case_multi_damage.get_module_name(), "TestData_MultipleDamage");
    EXPECT_TRUE(test_case_multi_damage.init_module(test_multi_damage));
    EXPECT_TRUE(test_case_multi_damage.test_get_damage_type().has_value());
    EXPECT_EQ(test_case_multi_damage.test_get_damage_type().value() & Damage::MAGIC, Damage::MAGIC);
    EXPECT_EQ(test_case_multi_damage.test_get_damage_type().value() & Damage::FIRE, Damage::FIRE);
    EXPECT_TRUE(test_case_multi_damage.test_get_amount().has_value());
    EXPECT_FLOAT_EQ(test_case_multi_damage.test_get_amount().value(), 1.4f);

    YAML::Node test_no_damage = modules["TestData_NoDamage"];
    EXPECT_TRUE(test_no_damage);
    EXPECT_TRUE(test_no_damage.IsMap());
    DamageTypeModule_Common_Test test_case_no_damage("TestData_NoDamage");
    EXPECT_EQ(test_case_no_damage.get_module_name(), "TestData_NoDamage");
    EXPECT_FALSE(test_case_no_damage.init_module(test_no_damage));
    EXPECT_FALSE(test_case_no_damage.test_get_damage_type().has_value());
    EXPECT_FALSE(test_case_no_damage.test_get_amount().has_value());

    YAML::Node test_damage_not_seq = modules["TestData_DamageNotSequence"];
    EXPECT_TRUE(test_damage_not_seq);
    EXPECT_TRUE(test_damage_not_seq.IsMap());
    DamageTypeModule_Common_Test test_case_damage_not_seq("TestData_DamageNotSequence");
    EXPECT_EQ(test_case_damage_not_seq.get_module_name(), "TestData_DamageNotSequence");
    EXPECT_FALSE(test_case_damage_not_seq.init_module(test_damage_not_seq));
    EXPECT_FALSE(test_case_damage_not_seq.test_get_damage_type().has_value());
    EXPECT_FALSE(test_case_damage_not_seq.test_get_amount().has_value());

    YAML::Node test_damage_invalid = modules["TestData_DamageInvalidType"];
    EXPECT_TRUE(test_damage_invalid);
    EXPECT_TRUE(test_damage_invalid.IsMap());
    DamageTypeModule_Common_Test test_case_damage_invalid("TestData_DamageInvalidType");
    EXPECT_EQ(test_case_damage_invalid.get_module_name(), "TestData_DamageInvalidType");
    EXPECT_FALSE(test_case_damage_invalid.init_module(test_damage_invalid));
    EXPECT_FALSE(test_case_damage_invalid.test_get_damage_type().has_value());
    EXPECT_FALSE(test_case_damage_invalid.test_get_amount().has_value());

    YAML::Node test_no_amount = modules["TestData_NoAmount"];
    EXPECT_TRUE(test_no_amount);
    EXPECT_TRUE(test_no_amount.IsMap());
    DamageTypeModule_Common_Test test_case_no_amount("TestData_NoAmount");
    EXPECT_EQ(test_case_no_amount.get_module_name(), "TestData_NoAmount");
    EXPECT_FALSE(test_case_no_amount.init_module(test_no_amount));
    EXPECT_FALSE(test_case_no_amount.test_get_damage_type().has_value());
    EXPECT_FALSE(test_case_no_amount.test_get_amount().has_value());

    YAML::Node test_amount_not_float = modules["TestData_AmountNotFloat"];
    EXPECT_TRUE(test_amount_not_float);
    EXPECT_TRUE(test_amount_not_float.IsMap());
    DamageTypeModule_Common_Test test_case_amount_not_float("TestData_AmountNotFloat");
    EXPECT_EQ(test_case_amount_not_float.get_module_name(), "TestData_AmountNotFloat");
    EXPECT_FALSE(test_case_amount_not_float.init_module(test_amount_not_float));
    EXPECT_FALSE(test_case_amount_not_float.test_get_damage_type().has_value());
    EXPECT_FALSE(test_case_amount_not_float.test_get_amount().has_value());
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
