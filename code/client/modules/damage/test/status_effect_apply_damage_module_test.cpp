#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include "code/client/status_effect/status_effect.h"
#include "code/client/status_effect/status_effect_library.h"
#include "code/client/modules/descriptors/status_effect_descriptor.h"
#include "code/client/modules/damage/status_effect_apply_damage_module.h"

// This Test class is just being used to expose some data to the 
//  public for use in the Test functions. These are not meant to be available
//  in the normal api, so they are being wrapped in this class instead.
class StatusEffectApply_DamageModule_Test : public Module::StatusEffectApply_DamageModule {
public:
    explicit StatusEffectApply_DamageModule_Test(const std::string& name, const YAML::Node& data) : StatusEffectApply_DamageModule(name, data) {}

    std::optional<std::string> test_get_status_effect_name() {
        Module::StatusEffect_Descriptor* desc = get_descriptor<Module::StatusEffect_Descriptor>();
        if (desc != nullptr) {
            return desc->get_status_effect_name(); 
        }
        return std::nullopt;
    }

    std::optional<Status::StatusEffectType> test_get_status_effect_type() {
        Module::StatusEffect_Descriptor* desc = get_descriptor<Module::StatusEffect_Descriptor>();
        if (desc != nullptr) {
            return desc->get_status_effect_type(); 
        }
        return std::nullopt;
    }
};

std::string k_yaml_file = "code\\client\\modules\\damage\\test\\data\\test_status_effect_apply_module.yaml";
std::string k_yaml_library_file = "code\\client\\modules\\damage\\test\\data\\test_status_effect_library_data.yaml";

TEST(StatusEffectApply_DamageModule, Test_StatusEffectApplyModule_Apply_StatusName) {
    Status::StatusEffectLibrary::get_Instance()->init_from_file(k_yaml_library_file);

    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_statusName = modules["TestData_StatusName"];
    EXPECT_TRUE(test_statusName);
    EXPECT_TRUE(test_statusName.IsMap());
    StatusEffectApply_DamageModule_Test test_case_statusName("TestData_StatusName", test_statusName);
    EXPECT_EQ(test_case_statusName.get_module_name(), "TestData_StatusName");
    EXPECT_TRUE(test_case_statusName.test_get_status_effect_name().has_value());
    EXPECT_EQ(test_case_statusName.test_get_status_effect_name().value(), "Ignite_V1");
    EXPECT_FALSE(test_case_statusName.test_get_status_effect_type().has_value());

    code::client::messages::Damage test_output;
    test_case_statusName.apply_damage(test_output);
    EXPECT_EQ(test_output.damage_type(), code::client::messages::Damage::NONE);
    EXPECT_FLOAT_EQ(test_output.amount(), 0.0f);
    EXPECT_EQ(test_output.status_effect_size(), 1);
    EXPECT_EQ(test_output.status_effect(0), "Ignite_V1");
    
    test_case_statusName.apply_damage(test_output);
    EXPECT_EQ(test_output.damage_type(), code::client::messages::Damage::NONE);
    EXPECT_FLOAT_EQ(test_output.amount(), 0.0f);
    EXPECT_EQ(test_output.status_effect_size(), 2);
    EXPECT_EQ(test_output.status_effect(0), "Ignite_V1");
    EXPECT_EQ(test_output.status_effect(1), "Ignite_V1");

    code::client::messages::Damage test_output_two;
    test_output_two.set_damage_type(code::client::messages::Damage::FIRE);
    test_output_two.set_amount(3.2f);
    test_case_statusName.apply_damage(test_output_two);
    EXPECT_EQ(test_output_two.damage_type() & code::client::messages::Damage::FIRE,
                code::client::messages::Damage::FIRE);
    EXPECT_FLOAT_EQ(test_output_two.amount(), 3.2f);
    EXPECT_EQ(test_output_two.status_effect_size(), 1);
    EXPECT_EQ(test_output_two.status_effect(0), "Ignite_V1");

    Status::StatusEffectLibrary::get_Instance()->shutdown();
}

TEST(StatusEffectApply_DamageModule, Test_StatusEffectApplyModule_Apply_StatusName_DoesntExist) {
    Status::StatusEffectLibrary::get_Instance()->init_from_file(k_yaml_library_file);

    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_statusName_doesntExist = modules["TestData_StatusName_DoesntExist"];
    EXPECT_TRUE(test_statusName_doesntExist);
    EXPECT_TRUE(test_statusName_doesntExist.IsMap());
    StatusEffectApply_DamageModule_Test test_case_statusName_doesntExist("TestData_StatusName_DoesntExist", test_statusName_doesntExist);
    EXPECT_EQ(test_case_statusName_doesntExist.get_module_name(), "TestData_StatusName_DoesntExist");
    EXPECT_FALSE(test_case_statusName_doesntExist.test_get_status_effect_name().has_value());
    EXPECT_FALSE(test_case_statusName_doesntExist.test_get_status_effect_type().has_value());

    code::client::messages::Damage test_output;
    test_case_statusName_doesntExist.apply_damage(test_output);
    EXPECT_EQ(test_output.damage_type(), code::client::messages::Damage::NONE);
    EXPECT_FLOAT_EQ(test_output.amount(), 0.0f);
    EXPECT_EQ(test_output.status_effect_size(), 0);

    code::client::messages::Damage test_output_two;
    test_output_two.set_damage_type(code::client::messages::Damage::FIRE);
    test_output_two.set_amount(3.2f);
    test_case_statusName_doesntExist.apply_damage(test_output_two);
    EXPECT_EQ(test_output_two.damage_type() & code::client::messages::Damage::FIRE,
                code::client::messages::Damage::FIRE);
    EXPECT_FLOAT_EQ(test_output_two.amount(), 3.2f);
    EXPECT_EQ(test_output_two.status_effect_size(), 0);

    Status::StatusEffectLibrary::get_Instance()->shutdown();
}

TEST(StatusEffectApply_DamageModule, Test_StatusEffectApplyModule_Apply_StatusType) {
    Status::StatusEffectLibrary::get_Instance()->init_from_file(k_yaml_library_file);

    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_statusType = modules["TestData_StatusType"];
    EXPECT_TRUE(test_statusType);
    EXPECT_TRUE(test_statusType.IsMap());
    StatusEffectApply_DamageModule_Test test_case_statusType("TestData_StatusType", test_statusType);
    EXPECT_EQ(test_case_statusType.get_module_name(), "TestData_StatusType");
    EXPECT_FALSE(test_case_statusType.test_get_status_effect_name().has_value());
    EXPECT_TRUE(test_case_statusType.test_get_status_effect_type().has_value());

    code::client::messages::Damage test_output;
    test_case_statusType.apply_damage(test_output);
    EXPECT_EQ(test_output.damage_type(), code::client::messages::Damage::NONE);
    EXPECT_FLOAT_EQ(test_output.amount(), 0.0f);
    EXPECT_EQ(test_output.status_effect_size(), 0);

    code::client::messages::Damage test_output_two;
    test_output_two.set_damage_type(code::client::messages::Damage::FIRE);
    test_output_two.set_amount(3.2f);
    test_case_statusType.apply_damage(test_output_two);
    EXPECT_EQ(test_output_two.damage_type() & code::client::messages::Damage::FIRE,
                code::client::messages::Damage::FIRE);
    EXPECT_FLOAT_EQ(test_output_two.amount(), 3.2f);
    EXPECT_EQ(test_output_two.status_effect_size(), 0);

    Status::StatusEffectLibrary::get_Instance()->shutdown();
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
