#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include "code/client/status_effect/status_effect.h"
#include "code/client/status_effect/status_effect_library.h"
#include "code/client/modules/descriptors/base/status_effect_descriptor_base.h"

// This Test class is just being used to expose some data to the 
//  public for use in the Test functions. These are not meant to be available
//  in the normal api, so they are being wrapped in this class instead.
class StatusEffectDescriptor_Base_Test : public Module::StatusEffectDescriptor_Base {
public:
    StatusEffectDescriptor_Base_Test() : StatusEffectDescriptor_Base() {}

    std::optional<std::string> test_get_status_effect_name() { return m_status_effect_name; }
    std::optional<Status::StatusEffectType> test_get_status_effect_type() { return m_status_effect_type; }
};

std::string k_yaml_file = "code\\client\\modules\\descriptors\\base\\test\\data\\status_effect.yaml";
std::string k_yaml_library_file = "code\\client\\modules\\descriptors\\base\\test\\data\\status_effect_lib.yaml";

TEST(StatusEffectDescriptor_Base, Test_StatusEffectDescriptorCreation_StatusName) {
    Status::StatusEffectLibrary::get_Instance()->init_from_file(k_yaml_library_file);

    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_statusName = modules["TestData_StatusName"];
    EXPECT_TRUE(test_statusName);
    EXPECT_TRUE(test_statusName.IsMap());
    StatusEffectDescriptor_Base_Test test_case_statusName;
    EXPECT_TRUE(test_case_statusName.init_descriptor(test_statusName));
    EXPECT_TRUE(test_case_statusName.test_get_status_effect_name().has_value());
    EXPECT_EQ(test_case_statusName.test_get_status_effect_name().value(), "Venom_V1");
    EXPECT_FALSE(test_case_statusName.test_get_status_effect_type().has_value());

    Status::StatusEffectLibrary::get_Instance()->shutdown();
}

TEST(StatusEffectDescriptor_Base, Test_StatusEffectDescriptorCreation_StatusName_DoesntExist) {
    Status::StatusEffectLibrary::get_Instance()->init_from_file(k_yaml_library_file);

    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_statusName_doesntExist = modules["TestData_StatusName_DoesntExist"];
    EXPECT_TRUE(test_statusName_doesntExist);
    EXPECT_TRUE(test_statusName_doesntExist.IsMap());
    StatusEffectDescriptor_Base_Test test_case_statusName_doesntExist;
    EXPECT_FALSE(test_case_statusName_doesntExist.init_descriptor(test_statusName_doesntExist));
    EXPECT_FALSE(test_case_statusName_doesntExist.test_get_status_effect_name().has_value());
    EXPECT_FALSE(test_case_statusName_doesntExist.test_get_status_effect_type().has_value());

    Status::StatusEffectLibrary::get_Instance()->shutdown();
}

TEST(StatusEffectDescriptor_Base, Test_StatusEffectDescriptorCreation_StatusType) {
    Status::StatusEffectLibrary::get_Instance()->init_from_file(k_yaml_library_file);

    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_statusType = modules["TestData_StatusType"];
    EXPECT_TRUE(test_statusType);
    EXPECT_TRUE(test_statusType.IsMap());
    StatusEffectDescriptor_Base_Test test_case_statusType;
    EXPECT_TRUE(test_case_statusType.init_descriptor(test_statusType));
    EXPECT_FALSE(test_case_statusType.test_get_status_effect_name().has_value());
    EXPECT_TRUE(test_case_statusType.test_get_status_effect_type().has_value());
    EXPECT_EQ(test_case_statusType.test_get_status_effect_type().value(), Status::StatusEffectType::POISON);

    Status::StatusEffectLibrary::get_Instance()->shutdown();
}

TEST(StatusEffectDescriptor_Base, Test_StatusEffectDescriptorCreation_StatusTypeInvalid) {
    Status::StatusEffectLibrary::get_Instance()->init_from_file(k_yaml_library_file);

    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_statusType_invalid = modules["TestData_StatusTypeInvalid"];
    EXPECT_TRUE(test_statusType_invalid);
    EXPECT_TRUE(test_statusType_invalid.IsMap());
    StatusEffectDescriptor_Base_Test test_case_statusType_invalid;
    EXPECT_FALSE(test_case_statusType_invalid.init_descriptor(test_statusType_invalid));
    EXPECT_FALSE(test_case_statusType_invalid.test_get_status_effect_name().has_value());
    EXPECT_FALSE(test_case_statusType_invalid.test_get_status_effect_type().has_value());

    Status::StatusEffectLibrary::get_Instance()->shutdown();
}

TEST(StatusEffectDescriptor_Base, Test_StatusEffectDescriptorCreation_TooManyFields) {
    Status::StatusEffectLibrary::get_Instance()->init_from_file(k_yaml_library_file);

    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_statusTooMany = modules["TestData_TooManyFields"];
    EXPECT_TRUE(test_statusTooMany);
    EXPECT_TRUE(test_statusTooMany.IsMap());
    StatusEffectDescriptor_Base_Test test_case_statusTooMany;
    EXPECT_FALSE(test_case_statusTooMany.init_descriptor(test_statusTooMany));
    EXPECT_FALSE(test_case_statusTooMany.test_get_status_effect_name().has_value());
    EXPECT_FALSE(test_case_statusTooMany.test_get_status_effect_type().has_value());

    Status::StatusEffectLibrary::get_Instance()->shutdown();
}

TEST(StatusEffectDescriptor_Base, Test_StatusEffectDescriptorCreation_NoFields) {
    Status::StatusEffectLibrary::get_Instance()->init_from_file(k_yaml_library_file);

    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_statusNoFields = modules["TestData_NoFields"];
    EXPECT_TRUE(test_statusNoFields);
    EXPECT_FALSE(test_statusNoFields.IsMap());
    StatusEffectDescriptor_Base_Test test_case_statusNoFields;
    EXPECT_FALSE(test_case_statusNoFields.init_descriptor(test_statusNoFields));
    EXPECT_FALSE(test_case_statusNoFields.test_get_status_effect_name().has_value());
    EXPECT_FALSE(test_case_statusNoFields.test_get_status_effect_type().has_value());

    Status::StatusEffectLibrary::get_Instance()->shutdown();
}

TEST(StatusEffectDescriptor_Base, Test_StatusEffectDescriptorCreation_NoValidFields) {
    Status::StatusEffectLibrary::get_Instance()->init_from_file(k_yaml_library_file);

    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_statusNoValidFields = modules["TestData_NoValidFields"];
    EXPECT_TRUE(test_statusNoValidFields);
    EXPECT_TRUE(test_statusNoValidFields.IsMap());
    StatusEffectDescriptor_Base_Test test_case_statusNoValidFields;
    EXPECT_FALSE(test_case_statusNoValidFields.init_descriptor(test_statusNoValidFields));
    EXPECT_FALSE(test_case_statusNoValidFields.test_get_status_effect_name().has_value());
    EXPECT_FALSE(test_case_statusNoValidFields.test_get_status_effect_type().has_value());

    Status::StatusEffectLibrary::get_Instance()->shutdown();
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
