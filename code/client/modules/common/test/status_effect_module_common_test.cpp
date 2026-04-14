#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include "code/client/status_effect/status_effect.h"
#include "code/client/status_effect/status_effect_library.h"
#include "code/client/modules/common/status_effect_module_common.h"

// This Test class is just being used to expose some data to the 
//  public for use in the Test functions. These are not meant to be available
//  in the normal api, so they are being wrapped in this class instead.
class StatusEffectModule_Common_Test : public Module::StatusEffectModule_Common {
public:
    explicit StatusEffectModule_Common_Test(const std::string& name) : StatusEffectModule_Common(name) {}
    
    std::optional<std::string> test_get_status_effect_name() { return m_status_effect_name; }
    std::optional<Status::StatusEffectType> test_get_status_effect_type() { return m_status_effect_type; }
};

std::string k_yaml_file = "code\\client\\modules\\common\\test\\data\\test_status_effect_module_common.yaml";
std::string k_yaml_library_file = "code\\client\\modules\\common\\test\\data\\test_status_effect_library_data.yaml";

TEST(StatusEffectModule_Common, Test_StatusEffectModuleCreation_StatusName) {
    Status::StatusEffectLibrary::get_Instance()->init_from_file(k_yaml_library_file);

    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_statusName = modules["TestData_StatusName"];
    EXPECT_TRUE(test_statusName);
    EXPECT_TRUE(test_statusName.IsMap());
    StatusEffectModule_Common_Test test_case_statusName("TestData_StatusName");
    EXPECT_EQ(test_case_statusName.get_module_name(), "TestData_StatusName");
    EXPECT_TRUE(test_case_statusName.init_module(test_statusName));
    EXPECT_TRUE(test_case_statusName.test_get_status_effect_name().has_value());
    EXPECT_EQ(test_case_statusName.test_get_status_effect_name().value(), "Venom_V1");
    EXPECT_FALSE(test_case_statusName.test_get_status_effect_type().has_value());

    Status::StatusEffectLibrary::get_Instance()->shutdown();
}

TEST(StatusEffectModule_Common, Test_StatusEffectModuleCreation_StatusName_DoesntExist) {
    Status::StatusEffectLibrary::get_Instance()->init_from_file(k_yaml_library_file);

    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_statusName_doesntExist = modules["TestData_StatusName_DoesntExist"];
    EXPECT_TRUE(test_statusName_doesntExist);
    EXPECT_TRUE(test_statusName_doesntExist.IsMap());
    StatusEffectModule_Common_Test test_case_statusName_doesntExist("TestData_StatusName_DoesntExist");
    EXPECT_EQ(test_case_statusName_doesntExist.get_module_name(), "TestData_StatusName_DoesntExist");
    EXPECT_FALSE(test_case_statusName_doesntExist.init_module(test_statusName_doesntExist));
    EXPECT_FALSE(test_case_statusName_doesntExist.test_get_status_effect_name().has_value());
    EXPECT_FALSE(test_case_statusName_doesntExist.test_get_status_effect_type().has_value());

    Status::StatusEffectLibrary::get_Instance()->shutdown();
}

TEST(StatusEffectModule_Common, Test_StatusEffectModuleCreation_StatusType) {
    Status::StatusEffectLibrary::get_Instance()->init_from_file(k_yaml_library_file);

    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_statusType = modules["TestData_StatusType"];
    EXPECT_TRUE(test_statusType);
    EXPECT_TRUE(test_statusType.IsMap());
    StatusEffectModule_Common_Test test_case_statusType("TestData_StatusType");
    EXPECT_EQ(test_case_statusType.get_module_name(), "TestData_StatusType");
    EXPECT_TRUE(test_case_statusType.init_module(test_statusType));
    EXPECT_FALSE(test_case_statusType.test_get_status_effect_name().has_value());
    EXPECT_TRUE(test_case_statusType.test_get_status_effect_type().has_value());
    EXPECT_EQ(test_case_statusType.test_get_status_effect_type().value(), Status::StatusEffectType::POISON);

    Status::StatusEffectLibrary::get_Instance()->shutdown();
}

TEST(StatusEffectModule_Common, Test_StatusEffectModuleCreation_StatusTypeInvalid) {
    Status::StatusEffectLibrary::get_Instance()->init_from_file(k_yaml_library_file);

    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_statusType_invalid = modules["TestData_StatusTypeInvalid"];
    EXPECT_TRUE(test_statusType_invalid);
    EXPECT_TRUE(test_statusType_invalid.IsMap());
    StatusEffectModule_Common_Test test_case_statusType_invalid("TestData_StatusTypeInvalid");
    EXPECT_EQ(test_case_statusType_invalid.get_module_name(), "TestData_StatusTypeInvalid");
    EXPECT_FALSE(test_case_statusType_invalid.init_module(test_statusType_invalid));
    EXPECT_FALSE(test_case_statusType_invalid.test_get_status_effect_name().has_value());
    EXPECT_FALSE(test_case_statusType_invalid.test_get_status_effect_type().has_value());

    Status::StatusEffectLibrary::get_Instance()->shutdown();
}

TEST(StatusEffectModule_Common, Test_StatusEffectModuleCreation_TooManyFields) {
    Status::StatusEffectLibrary::get_Instance()->init_from_file(k_yaml_library_file);

    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_statusTooMany = modules["TestData_TooManyFields"];
    EXPECT_TRUE(test_statusTooMany);
    EXPECT_TRUE(test_statusTooMany.IsMap());
    StatusEffectModule_Common_Test test_case_statusTooMany("TestData_TooManyFields");
    EXPECT_EQ(test_case_statusTooMany.get_module_name(), "TestData_TooManyFields");
    EXPECT_FALSE(test_case_statusTooMany.init_module(test_statusTooMany));
    EXPECT_FALSE(test_case_statusTooMany.test_get_status_effect_name().has_value());
    EXPECT_FALSE(test_case_statusTooMany.test_get_status_effect_type().has_value());

    Status::StatusEffectLibrary::get_Instance()->shutdown();
}

TEST(StatusEffectModule_Common, Test_StatusEffectModuleCreation_NoFields) {
    Status::StatusEffectLibrary::get_Instance()->init_from_file(k_yaml_library_file);

    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_statusNoFields = modules["TestData_NoFields"];
    EXPECT_TRUE(test_statusNoFields);
    EXPECT_FALSE(test_statusNoFields.IsMap());
    StatusEffectModule_Common_Test test_case_statusNoFields("TestData_NoFields");
    EXPECT_EQ(test_case_statusNoFields.get_module_name(), "TestData_NoFields");
    EXPECT_FALSE(test_case_statusNoFields.init_module(test_statusNoFields));
    EXPECT_FALSE(test_case_statusNoFields.test_get_status_effect_name().has_value());
    EXPECT_FALSE(test_case_statusNoFields.test_get_status_effect_type().has_value());

    Status::StatusEffectLibrary::get_Instance()->shutdown();
}

TEST(StatusEffectModule_Common, Test_StatusEffectModuleCreation_NoValidFields) {
    Status::StatusEffectLibrary::get_Instance()->init_from_file(k_yaml_library_file);

    YAML::Node modules = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(modules.IsMap());

    YAML::Node test_statusNoValidFields = modules["TestData_NoValidFields"];
    EXPECT_TRUE(test_statusNoValidFields);
    EXPECT_TRUE(test_statusNoValidFields.IsMap());
    StatusEffectModule_Common_Test test_case_statusNoValidFields("TestData_NoValidFields");
    EXPECT_EQ(test_case_statusNoValidFields.get_module_name(), "TestData_NoValidFields");
    EXPECT_FALSE(test_case_statusNoValidFields.init_module(test_statusNoValidFields));
    EXPECT_FALSE(test_case_statusNoValidFields.test_get_status_effect_name().has_value());
    EXPECT_FALSE(test_case_statusNoValidFields.test_get_status_effect_type().has_value());

    Status::StatusEffectLibrary::get_Instance()->shutdown();
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
