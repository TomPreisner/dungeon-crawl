#include <gtest/gtest.h>

#include <typeinfo>
#include <iostream>
#include <string>
#include <set>
#include "code/client/health/health_manager.h"
#include "code/client/modules/base/base_module.h"
#include "code/client/modules/health/damage_type_resist_health_module.h"
#include "code/client/modules/health/status_effect_immune_health_module.h"
#include "code/core/message_system/message_switchboard.h"

class HealthManagerTest : public HealthManager {
public:
    const HealthModuleMap& public_test_get_health_modules() const { return test_get_health_modules(); }
    const std::string& public_test_get_uuid_string() const { return test_get_uuid_string(); }
};

std::string k_yaml_file = "code\\client\\health\\test\\data\\test_health_manager_data.yaml";

TEST(HealthManager_Test, Test_HealthManager_Init) {
    core::MessageSwitchboard switchboard;
    std::shared_ptr<HealthManagerTest> health_manager;

    // A simple load of a yaml file and validate the contents
    YAML::Node data_set = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(data_set.IsMap());

    // Create the health managers and check that they initialize as expected
    health_manager = std::make_shared<HealthManagerTest>();
    EXPECT_TRUE(health_manager->init_manager(switchboard, data_set["TestData_Health_Manager"]));
        // can't reinitialize
    EXPECT_FALSE(health_manager->init_manager(switchboard, data_set["TestData_Health_Manager"]));

    health_manager = std::make_shared<HealthManagerTest>();
    EXPECT_FALSE(health_manager->init_manager(switchboard, data_set["TestData_Health_Manager_Negative_Total"]));

    health_manager = std::make_shared<HealthManagerTest>();
    EXPECT_FALSE(health_manager->init_manager(switchboard, data_set["TestData_Health_Manager_Negative_Health"]));

    health_manager = std::make_shared<HealthManagerTest>();
    EXPECT_FALSE(health_manager->init_manager(switchboard, data_set["TestData_Health_Manager_Zero_Total"]));

    health_manager = std::make_shared<HealthManagerTest>();
    EXPECT_TRUE(health_manager->init_manager(switchboard, data_set["TestData_Health_Manager_Zero_Health"]));
}

TEST(HealthManager_Test, Test_HealthManager_DefaultModules) {
    // Test that the default modules load properly
    core::MessageSwitchboard switchboard;
    std::shared_ptr<HealthManagerTest> health_manager;

    // A simple load of a yaml file and validate the contents
    YAML::Node data_set = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(data_set.IsMap());
    health_manager = std::make_shared<HealthManagerTest>();
    EXPECT_TRUE(health_manager->init_manager(switchboard, data_set["TestData_Health_Manager"]));

    EXPECT_EQ(health_manager->public_test_get_health_modules().size(), 1);
    auto iter = health_manager->public_test_get_health_modules().find(health_manager->public_test_get_uuid_string());
    EXPECT_TRUE(iter != health_manager->public_test_get_health_modules().cend());
    EXPECT_EQ(iter->second.size(), 2);

    const auto end = iter->second.cend();
    for (auto curr = iter->second.begin(); curr != end; ++curr) {
        Module::HealthModule* base_module_ptr = dynamic_cast<Module::HealthModule*>(curr->get());
        EXPECT_TRUE(base_module_ptr != nullptr);
        if (base_module_ptr != nullptr) {
            EXPECT_TRUE((base_module_ptr->get_module_name() == "StatusEffectImmune") ||
                        (base_module_ptr->get_module_name() == "DamageResistAmount"));

            // Make sure that the casting to the derived types works and the vtables are maintained through creation
            if (base_module_ptr->get_module_name() == "StatusEffectImmune") {
                EXPECT_NE(dynamic_cast<Module::StatusEffectImmune_HealthModule*>(curr->get()), nullptr);
            }
            if (base_module_ptr->get_module_name() == "DamageResistAmount") {
                EXPECT_NE(dynamic_cast<Module::DamageTypeResist_HealthModule*>(curr->get()), nullptr);
            }
        }
    }
}

TEST(HealthManager_Test, Test_HealthManager_AddRemoveModules) {
    // Test that the default modules load properly
    core::MessageSwitchboard switchboard;
    std::shared_ptr<HealthManagerTest> health_manager;

    // A simple load of a yaml file and validate the contents
    YAML::Node data_set = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(data_set.IsMap());
    health_manager = std::make_shared<HealthManagerTest>();
    EXPECT_TRUE(health_manager->init_manager(switchboard, data_set["TestData_Health_Manager"]));

    const auto end = health_manager->public_test_get_health_modules().cend();
    for (auto iter = health_manager->public_test_get_health_modules().begin(); iter != end; ++iter) {
        EXPECT_EQ(iter->first, health_manager->public_test_get_uuid_string());
    }

    EXPECT_EQ(health_manager->public_test_get_health_modules().size(), 1);
    auto iter = health_manager->public_test_get_health_modules().find(health_manager->public_test_get_uuid_string());
    EXPECT_TRUE(iter != health_manager->public_test_get_health_modules().cend());
    EXPECT_EQ(iter->second.size(), 2);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
