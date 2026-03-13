#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include "code/client/status_effect/status_effect_library.h"
#include "code/client/status_effect/status_effect_manager.h"

std::string k_yaml_file = "code\\client\\status_effect\\test\\data\\test_status_effect_library_data.yaml";

// This Test class is just being used to expose some functions to the 
//  public for use in the Test functions. These are not meant to be available
//  in the normal api, so they are being wrapped in this class instead.
class StatusEffectManager_Test : public Status::StatusEffectManager {
public:
    explicit StatusEffectManager_Test() : StatusEffectManager() {}
    
    const std::list<Status::StatusEffect>& public_test_get_status_effects() const {
        return test_get_status_effects(); 
    } 
    const std::list<Status::StatusEffectManager::StatusEffectAction>& public_test_get_pending_status_effect_actions() const {
        return test_get_pending_status_effect_actions();
    } 
};

TEST(StatusEffectManager, Test_StatusEffectManager) {
    // A simple load of a yaml file and validate the contents
    {
        YAML::Node config = YAML::LoadFile(k_yaml_file);
        EXPECT_TRUE(config.IsMap());
    }

    // Initialize the status efect library because it is needed for the status effect manager's operation
    Status::StatusEffectLibrary::get_Instance()->init_from_file(k_yaml_file);

    StatusEffectManager_Test manager_test;
    EXPECT_TRUE(manager_test.public_test_get_status_effects().empty());
    EXPECT_TRUE(manager_test.public_test_get_pending_status_effect_actions().empty());

    EXPECT_FALSE(manager_test.apply_status_effect(""));
    EXPECT_TRUE(manager_test.public_test_get_pending_status_effect_actions().empty());
    EXPECT_FALSE(manager_test.apply_status_effect("FooBar"));
    EXPECT_TRUE(manager_test.public_test_get_pending_status_effect_actions().empty());
    manager_test.update_manager();
    EXPECT_TRUE(manager_test.public_test_get_status_effects().empty());

    EXPECT_TRUE(manager_test.apply_status_effect("Ignite_V1"));
    EXPECT_EQ(manager_test.public_test_get_pending_status_effect_actions().size(), 1);
    EXPECT_TRUE(manager_test.apply_status_effect("DeepCut_V1"));
    EXPECT_EQ(manager_test.public_test_get_pending_status_effect_actions().size(), 2);
    manager_test.update_manager();
    EXPECT_EQ(manager_test.public_test_get_status_effects().size(), 2);
    EXPECT_TRUE(manager_test.apply_status_effect("Venom_V1"));
    EXPECT_TRUE(manager_test.public_test_get_pending_status_effect_actions().size(), 1);
    manager_test.update_manager();
    EXPECT_EQ(manager_test.public_test_get_status_effects().size(), 3);
    EXPECT_TRUE(manager_test.public_test_get_pending_status_effect_actions().empty());
    // The same status effect can be applied multiple times
    EXPECT_TRUE(manager_test.apply_status_effect("Ignite_V1"));
    EXPECT_EQ(manager_test.public_test_get_pending_status_effect_actions().size(), 1);
    EXPECT_TRUE(manager_test.apply_status_effect("DeepCut_V1"));
    EXPECT_EQ(manager_test.public_test_get_pending_status_effect_actions().size(), 2);
    manager_test.update_manager();
    EXPECT_EQ(manager_test.public_test_get_status_effects().size(), 5);

    int count = 0;
    for (auto& status_effect : manager_test.public_test_get_status_effects()) {
        if (count >= 2) {
            break;
        }
        ++count;
        manager_test.clear_status_effect(status_effect.get_uuid());
        EXPECT_EQ(manager_test.public_test_get_pending_status_effect_actions().size(), count);
    }

    manager_test.clear_status_effect("");
    manager_test.clear_status_effect("FooBar");
    EXPECT_EQ(manager_test.public_test_get_pending_status_effect_actions().size(), 4);
    manager_test.update_manager();
    EXPECT_TRUE(manager_test.public_test_get_pending_status_effect_actions().empty());
    EXPECT_EQ(manager_test.public_test_get_status_effects().size(), 3);
    manager_test.clear_all_status_effects();    //< clear all will clear pending adds this update frame
    EXPECT_TRUE(manager_test.apply_status_effect("Venom_V1"));
    EXPECT_TRUE(manager_test.public_test_get_pending_status_effect_actions().size(), 2);
    EXPECT_TRUE(manager_test.apply_status_effect("DeepCut_V1"));
    EXPECT_TRUE(manager_test.public_test_get_pending_status_effect_actions().size(), 3);
    manager_test.update_manager();
    EXPECT_TRUE(manager_test.public_test_get_pending_status_effect_actions().empty());
    EXPECT_TRUE(manager_test.public_test_get_status_effects().empty());
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
