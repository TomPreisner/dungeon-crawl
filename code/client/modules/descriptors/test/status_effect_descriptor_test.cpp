#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include "code/client/status_effect/status_effect.h"
#include "code/client/status_effect/status_effect_library.h"
#include "code/client/modules/descriptors/status_effect_descriptor.h"

// This Test class is just being used to expose some data to the 
//  public for use in the Test functions. These are not meant to be available
//  in the normal api, so they are being wrapped in this class instead.
class StatusEffect_Descriptor_Test : public Module::StatusEffect_Descriptor {
public:
    StatusEffect_Descriptor_Test() : StatusEffect_Descriptor() {}

    std::optional<std::string> test_get_status_effect_name() { return m_status_effect_name; }
    std::optional<Status::StatusEffectType> test_get_status_effect_type() { return m_status_effect_type; }
    std::optional<float> test_get_percent_chance() { return m_percent_chance; }
};

std::string k_yaml_file = "code\\client\\modules\\descriptors\\test\\data\\test_status_effect_descriptor.yaml";
std::string k_yaml_library_file = "code\\client\\modules\\descriptors\\test\\data\\test_status_effect_library_data.yaml";

TEST(StatusEffect_Descriptor, Test_StatusEffectDescriptor_StatusType) {
    Status::StatusEffectLibrary::get_Instance()->init_from_file(k_yaml_library_file);

    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_statusType = modules["TestData_StatusType"];
    EXPECT_TRUE(test_statusType);
    EXPECT_TRUE(test_statusType.IsMap());
    StatusEffect_Descriptor_Test test_case_statusType;
    EXPECT_TRUE(test_case_statusType.init_descriptor(test_statusType));
    EXPECT_FALSE(test_case_statusType.test_get_status_effect_name().has_value());
    EXPECT_TRUE(test_case_statusType.test_get_status_effect_type().has_value());
    EXPECT_EQ(test_case_statusType.test_get_status_effect_type().value(), Status::StatusEffectType::FIRE);
    EXPECT_FALSE(test_case_statusType.test_get_percent_chance().has_value());

    Status::StatusEffectLibrary::get_Instance()->shutdown();
}

TEST(StatusEffect_Descriptor, Test_StatusEffectDescriptor_StatusType_Invalid) {
    Status::StatusEffectLibrary::get_Instance()->init_from_file(k_yaml_library_file);

    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_statusType_Invalid = modules["TestData_StatusType_Invalid"];
    EXPECT_TRUE(test_statusType_Invalid);
    EXPECT_TRUE(test_statusType_Invalid.IsMap());
    StatusEffect_Descriptor_Test test_case_statusType_Invalid;
    EXPECT_FALSE(test_case_statusType_Invalid.init_descriptor(test_statusType_Invalid));
    EXPECT_FALSE(test_case_statusType_Invalid.test_get_status_effect_name().has_value());
    EXPECT_FALSE(test_case_statusType_Invalid.test_get_status_effect_type().has_value());
    EXPECT_FALSE(test_case_statusType_Invalid.test_get_percent_chance().has_value());

    Status::StatusEffectLibrary::get_Instance()->shutdown();
}

TEST(StatusEffect_Descriptor, Test_StatusEffectDescriptor_StatusType_50_Percent) {
    Status::StatusEffectLibrary::get_Instance()->init_from_file(k_yaml_library_file);

    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_statusType_50 = modules["TestData_StatusType_50_Percent"];
    EXPECT_TRUE(test_statusType_50);
    EXPECT_TRUE(test_statusType_50.IsMap());
    StatusEffect_Descriptor_Test test_case_statusType_50;
    EXPECT_TRUE(test_case_statusType_50.init_descriptor(test_statusType_50));
    EXPECT_FALSE(test_case_statusType_50.test_get_status_effect_name().has_value());
    EXPECT_TRUE(test_case_statusType_50.test_get_status_effect_type().has_value());
    EXPECT_EQ(test_case_statusType_50.test_get_status_effect_type().value(), Status::StatusEffectType::POISON);
    EXPECT_TRUE(test_case_statusType_50.test_get_percent_chance().has_value());
    EXPECT_FLOAT_EQ(test_case_statusType_50.test_get_percent_chance().value(), 0.5f);

    Status::StatusEffectLibrary::get_Instance()->shutdown();
}

TEST(StatusEffect_Descriptor, Test_StatusEffectDescriptor_StatusType_NegativePercent) {
    Status::StatusEffectLibrary::get_Instance()->init_from_file(k_yaml_library_file);

    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_statusType_NegativePercent = modules["TestData_StatusType_Negative_Percent"];
    EXPECT_TRUE(test_statusType_NegativePercent);
    EXPECT_TRUE(test_statusType_NegativePercent.IsMap());
    StatusEffect_Descriptor_Test test_case_statusType_NegativePercent;
    EXPECT_FALSE(test_case_statusType_NegativePercent.init_descriptor(test_statusType_NegativePercent));
    EXPECT_FALSE(test_case_statusType_NegativePercent.test_get_status_effect_name().has_value());
    EXPECT_FALSE(test_case_statusType_NegativePercent.test_get_status_effect_type().has_value());
    EXPECT_FALSE(test_case_statusType_NegativePercent.test_get_percent_chance().has_value());

    Status::StatusEffectLibrary::get_Instance()->shutdown();
}

TEST(StatusEffect_Descriptor, Test_StatusEffectDescriptor_StatusType_TooMuchPercent) {
    Status::StatusEffectLibrary::get_Instance()->init_from_file(k_yaml_library_file);

    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_statusType_TooMuchPercent = modules["TestData_StatusType_TooMuch_Percent"];
    EXPECT_TRUE(test_statusType_TooMuchPercent);
    EXPECT_TRUE(test_statusType_TooMuchPercent.IsMap());
    StatusEffect_Descriptor_Test test_case_statusType_TooMuchPercent;
    EXPECT_FALSE(test_case_statusType_TooMuchPercent.init_descriptor(test_statusType_TooMuchPercent));
    EXPECT_FALSE(test_case_statusType_TooMuchPercent.test_get_status_effect_name().has_value());
    EXPECT_FALSE(test_case_statusType_TooMuchPercent.test_get_status_effect_type().has_value());
    EXPECT_FALSE(test_case_statusType_TooMuchPercent.test_get_percent_chance().has_value());

    Status::StatusEffectLibrary::get_Instance()->shutdown();
}

TEST(StatusEffect_Descriptor, Test_StatusEffectDescriptor_StatusType_ZeroPercent) {
    Status::StatusEffectLibrary::get_Instance()->init_from_file(k_yaml_library_file);

    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_statusType_ZeroPercent = modules["TestData_StatusType_Zero_Percent"];
    EXPECT_TRUE(test_statusType_ZeroPercent);
    EXPECT_TRUE(test_statusType_ZeroPercent.IsMap());
    StatusEffect_Descriptor_Test test_case_statusType_ZeroPercent;
    EXPECT_FALSE(test_case_statusType_ZeroPercent.init_descriptor(test_statusType_ZeroPercent));
    EXPECT_FALSE(test_case_statusType_ZeroPercent.test_get_status_effect_name().has_value());
    EXPECT_FALSE(test_case_statusType_ZeroPercent.test_get_status_effect_type().has_value());
    EXPECT_FALSE(test_case_statusType_ZeroPercent.test_get_percent_chance().has_value());

    Status::StatusEffectLibrary::get_Instance()->shutdown();
}

TEST(StatusEffect_Descriptor, Test_StatusEffectDescriptor_StatusType_StatusName) {
    Status::StatusEffectLibrary::get_Instance()->init_from_file(k_yaml_library_file);

    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_StatusName = modules["TestData_StatusName"];
    EXPECT_TRUE(test_StatusName);
    EXPECT_TRUE(test_StatusName.IsMap());
    StatusEffect_Descriptor_Test test_case_StatusName;
    EXPECT_TRUE(test_case_StatusName.init_descriptor(test_StatusName));
    EXPECT_TRUE(test_case_StatusName.test_get_status_effect_name().has_value());
    EXPECT_EQ(test_case_StatusName.test_get_status_effect_name().value(), "Ignite_V1");
    EXPECT_FALSE(test_case_StatusName.test_get_status_effect_type().has_value());
    EXPECT_FALSE(test_case_StatusName.test_get_percent_chance().has_value());

    Status::StatusEffectLibrary::get_Instance()->shutdown();
}

TEST(StatusEffect_Descriptor, Test_StatusEffectDescriptor_StatusType_StatusName_DoesntExist) {
    Status::StatusEffectLibrary::get_Instance()->init_from_file(k_yaml_library_file);

    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_statusName_doesntExist = modules["TestData_StatusName_DoesntExist"];
    EXPECT_TRUE(test_statusName_doesntExist);
    EXPECT_TRUE(test_statusName_doesntExist.IsMap());
    StatusEffect_Descriptor_Test test_case_statusName_doesntExist;
    EXPECT_FALSE(test_case_statusName_doesntExist.init_descriptor(test_statusName_doesntExist));
    EXPECT_FALSE(test_case_statusName_doesntExist.test_get_status_effect_name().has_value());
    EXPECT_FALSE(test_case_statusName_doesntExist.test_get_status_effect_type().has_value());
    EXPECT_FALSE(test_case_statusName_doesntExist.test_get_percent_chance().has_value());

    Status::StatusEffectLibrary::get_Instance()->shutdown();
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
