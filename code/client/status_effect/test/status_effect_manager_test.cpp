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
    const std::queue<Status::StatusEffectManager::StatusEffectAction>& public_test_get_pending_status_effect_actions() const {
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

    const std::chrono::milliseconds update_rate(500); // 0.5 seconds
    StatusEffectManager_Test manager_test;
    EXPECT_TRUE(manager_test.public_test_get_status_effects().empty());
    EXPECT_TRUE(manager_test.public_test_get_pending_status_effect_actions().empty());

    EXPECT_FALSE(manager_test.apply_status_effect(""));
    EXPECT_TRUE(manager_test.public_test_get_pending_status_effect_actions().empty());
    EXPECT_FALSE(manager_test.apply_status_effect("FooBar"));
    EXPECT_TRUE(manager_test.public_test_get_pending_status_effect_actions().empty());
    manager_test.update_manager(update_rate);
    EXPECT_TRUE(manager_test.public_test_get_status_effects().empty());

    EXPECT_TRUE(manager_test.apply_status_effect("Ignite_V1"));
    EXPECT_EQ(manager_test.public_test_get_pending_status_effect_actions().size(), 1);
    EXPECT_TRUE(manager_test.apply_status_effect("DeepCut_V1"));
    EXPECT_EQ(manager_test.public_test_get_pending_status_effect_actions().size(), 2);
    manager_test.update_manager(update_rate);
    EXPECT_EQ(manager_test.public_test_get_status_effects().size(), 2);
    EXPECT_TRUE(manager_test.apply_status_effect("Venom_V1"));
    EXPECT_EQ(manager_test.public_test_get_pending_status_effect_actions().size(), 1);
    manager_test.update_manager(update_rate);
    EXPECT_EQ(manager_test.public_test_get_status_effects().size(), 3);
    EXPECT_TRUE(manager_test.public_test_get_pending_status_effect_actions().empty());
    // The same status effect can be applied multiple times
    EXPECT_TRUE(manager_test.apply_status_effect("Ignite_V1"));
    EXPECT_EQ(manager_test.public_test_get_pending_status_effect_actions().size(), 1);
    EXPECT_TRUE(manager_test.apply_status_effect("DeepCut_V1"));
    EXPECT_EQ(manager_test.public_test_get_pending_status_effect_actions().size(), 2);
    manager_test.update_manager(update_rate);
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
    manager_test.update_manager(update_rate);
    EXPECT_TRUE(manager_test.public_test_get_pending_status_effect_actions().empty());
    EXPECT_EQ(manager_test.public_test_get_status_effects().size(), 3);
    manager_test.clear_all_status_effects();    //< clear all will clear pending adds this update frame
    EXPECT_TRUE(manager_test.apply_status_effect("Venom_V1"));
    EXPECT_EQ(manager_test.public_test_get_pending_status_effect_actions().size(), 2);
    EXPECT_TRUE(manager_test.apply_status_effect("DeepCut_V1"));
    EXPECT_EQ(manager_test.public_test_get_pending_status_effect_actions().size(), 3);
    manager_test.update_manager(update_rate);
    EXPECT_TRUE(manager_test.public_test_get_pending_status_effect_actions().empty());
    EXPECT_TRUE(manager_test.public_test_get_status_effects().empty());

    Status::StatusEffectLibrary::get_Instance()->shutdown();
}

TEST(StatusEffectManager, Test_StatusEffectManager_MessageSubscriber_Test) {
    // The Status Effect Manager subscribes to 
    //  ApplyStatus
    //  ClearStatus

    // Set up the Library to allow for apply calls
    Status::StatusEffectLibrary::get_Instance()->init_from_file(k_yaml_file);

    core::MessageSwitchboard switchboard;
    StatusEffectManager_Test manager_test;
    manager_test.init_manager(switchboard);

    const std::chrono::milliseconds update_rate(500); // 0.5 seconds
    EXPECT_TRUE(manager_test.public_test_get_status_effects().empty());
    EXPECT_TRUE(manager_test.public_test_get_pending_status_effect_actions().empty());

    Messages::ApplyStatus apply_empty{""};
    Messages::ApplyStatus apply_non_existant{"FooBar"};
    Messages::ApplyStatus apply_ignite{"Ignite_V1"};
    Messages::ApplyStatus apply_deepcut{"DeepCut_V1"};
    Messages::ApplyStatus apply_venom{"Venom_V1"};

    // These should fall on the floor
    switchboard.publish_message(apply_empty);
    switchboard.publish_message(apply_non_existant);
    EXPECT_TRUE(manager_test.public_test_get_pending_status_effect_actions().empty());

    // try adding
    switchboard.publish_message(apply_ignite);
    switchboard.publish_message(apply_deepcut);
    EXPECT_EQ(manager_test.public_test_get_pending_status_effect_actions().size(), 2);
    manager_test.update_manager(update_rate);
    EXPECT_EQ(manager_test.public_test_get_status_effects().size(), 2);

    // add duplicates
    switchboard.publish_message(apply_venom);
    switchboard.publish_message(apply_ignite);
    switchboard.publish_message(apply_deepcut);
    EXPECT_EQ(manager_test.public_test_get_pending_status_effect_actions().size(), 3);
    manager_test.update_manager(update_rate);
    EXPECT_EQ(manager_test.public_test_get_status_effects().size(), 5);

    // clear two specific statuses one at a time. Clear both ignites.
    int num_ignites = 0;
    std::string uuid = "";
    for (auto& status_effect : manager_test.public_test_get_status_effects()) {
        if (status_effect.get_type() != Status::StatusEffectType::FIRE) {
            continue;
        }
        ++num_ignites;
        if (uuid.empty()) {
            uuid = status_effect.get_uuid();
        }
    }
    EXPECT_FALSE(uuid.empty());
    EXPECT_EQ(num_ignites, 2);
    Messages::ClearStatus clear_single{uuid};
    switchboard.publish_message(clear_single);
    EXPECT_EQ(manager_test.public_test_get_pending_status_effect_actions().size(), 1);
    manager_test.update_manager(update_rate);
    EXPECT_EQ(manager_test.public_test_get_status_effects().size(), 4);

    // clear invalid does nothing
    clear_single.status_uuid = {"Foobar-data-test-datatest"};
    switchboard.publish_message(clear_single);
    EXPECT_EQ(manager_test.public_test_get_pending_status_effect_actions().size(), 1);
    manager_test.update_manager(update_rate);
    EXPECT_EQ(manager_test.public_test_get_status_effects().size(), 4);

    // reset data
    num_ignites = 0;
    uuid = "";
    clear_single.status_uuid = "";

    for (auto& status_effect : manager_test.public_test_get_status_effects()) {
        if (status_effect.get_type() != Status::StatusEffectType::FIRE) {
            continue;
        }
        ++num_ignites;
        if (uuid.empty()) {
            uuid = status_effect.get_uuid();
        }
    }
    EXPECT_FALSE(uuid.empty());
    EXPECT_EQ(num_ignites, 1);

    clear_single.status_uuid = uuid;
    switchboard.publish_message(clear_single);
    EXPECT_EQ(manager_test.public_test_get_pending_status_effect_actions().size(), 1);
    manager_test.update_manager(update_rate);
    EXPECT_EQ(manager_test.public_test_get_status_effects().size(), 3);

    // reset data
    num_ignites = 0;
    uuid = "";

    for (auto& status_effect : manager_test.public_test_get_status_effects()) {
        if (status_effect.get_type() != Status::StatusEffectType::FIRE) {
            continue;
        }
        ++num_ignites;
        if (uuid.empty()) {
            uuid = status_effect.get_uuid();
        }
    }

    EXPECT_TRUE(uuid.empty());
    EXPECT_EQ(num_ignites, 0);

    // clear all and then the manager should be empty
    Messages::ClearStatus clear_all{""};
    switchboard.publish_message(clear_all);
    EXPECT_EQ(manager_test.public_test_get_pending_status_effect_actions().size(), 1);
    manager_test.update_manager(update_rate);
    EXPECT_EQ(manager_test.public_test_get_status_effects().size(), 0);

    Status::StatusEffectLibrary::get_Instance()->shutdown();
}

TEST(StatusEffectManager, Test_StatusEffectManager_MessagePublisher_Test) {
    // The Status Effect Manager publishes to 
    //  ApplyDirectHeal
    //  ApplyDirectDamage

    // Set up the Library to allow for apply calls
    Status::StatusEffectLibrary::get_Instance()->init_from_file(k_yaml_file);

    core::MessageSwitchboard switchboard;
    StatusEffectManager_Test manager_test;
    manager_test.init_manager(switchboard);

    const std::chrono::milliseconds update_rate(500); // 0.5 seconds
    EXPECT_TRUE(manager_test.public_test_get_status_effects().empty());
    EXPECT_TRUE(manager_test.public_test_get_pending_status_effect_actions().empty());

    Messages::ApplyStatus apply_ignite{"Ignite_V1"};

    // Create listeners for the heal and damage publishers
    int heal_count = 0;
    float heal_amount = 0.f;
    auto heal_subscriber = std::make_shared<core::MessageSubscriber<Messages::ApplyDirectHeal>>(switchboard);
    heal_subscriber->register_callback([&heal_amount, &heal_count](const Messages::ApplyDirectHeal& heal_msg) {
        heal_amount += heal_msg.amount;
        ++heal_count;
    });
    EXPECT_FLOAT_EQ(heal_amount, 0.f);
    EXPECT_EQ(heal_count, 0);

    int damage_count = 0;
    float damage_amount = 0.f;
    auto damage_subscriber = std::make_shared<core::MessageSubscriber<Messages::ApplyDirectDamage>>(switchboard);
    damage_subscriber->register_callback([&damage_amount, &damage_count](const Messages::ApplyDirectDamage& damage_msg) {
        damage_amount += damage_msg.amount;
        ++damage_count;
    });
    EXPECT_FLOAT_EQ(damage_amount, 0.f);
    EXPECT_EQ(damage_count, 0);

    switchboard.publish_message(apply_ignite);
    EXPECT_EQ(manager_test.public_test_get_pending_status_effect_actions().size(), 1);
    manager_test.update_manager(update_rate);
    EXPECT_EQ(manager_test.public_test_get_status_effects().size(), 1);

    manager_test.update_manager(update_rate);
    for (auto& status_effect : manager_test.public_test_get_status_effects()) {
        EXPECT_TRUE(status_effect.is_active());
    }
    // update twice to get the status effects to update
    manager_test.update_manager(update_rate);
    manager_test.update_manager(update_rate);

    EXPECT_FLOAT_EQ(heal_amount, 2.1f);
    EXPECT_EQ(heal_count, 1);
    EXPECT_FLOAT_EQ(damage_amount, 1.7f);
    EXPECT_EQ(damage_count, 1);

    // update twice to get the status effects to update
    manager_test.update_manager(update_rate);
    manager_test.update_manager(update_rate);

    EXPECT_FLOAT_EQ(heal_amount, 4.2f);
    EXPECT_EQ(heal_count, 2);
    EXPECT_FLOAT_EQ(damage_amount, 3.4f);
    EXPECT_EQ(damage_count, 2);

    Status::StatusEffectLibrary::get_Instance()->shutdown();
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
