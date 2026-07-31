#include <gtest/gtest.h>

#include <typeinfo>
#include <iostream>
#include <string>
#include <set>
#include "code/client/health/health_manager.h"
#include "code/client/modules/base/base_module.h"
#include "code/client/modules/health/damage_type_resist_health_module.h"
#include "code/client/modules/health/status_effect_immune_health_module.h"
#include "code/client/status_effect/status_effect_library.h"
#include "code/core/message_system/message_switchboard.h"

class HealthManagerTest : public HealthManager {
public:
    const HealthModuleMap& public_test_get_health_modules() const { return test_get_health_modules(); }
    const std::string& public_test_get_uuid_string() const { return test_get_uuid_string(); }
};

std::string k_yaml_file = "code\\client\\health\\test\\data\\test_health_manager_data.yaml";
std::string k_module_yaml_file = "code\\client\\health\\test\\data\\test_health_modules.yaml";
std::string k_yaml_library_file = "code\\client\\health\\test\\data\\test_status_effect_library_data.yaml";

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

    // This makes sure that the default assignment is correct
    EXPECT_EQ(health_manager->public_test_get_health_modules().size(), 1);
    auto iter = health_manager->public_test_get_health_modules().find(health_manager->public_test_get_uuid_string());
    EXPECT_TRUE(iter != health_manager->public_test_get_health_modules().cend());
    EXPECT_EQ(iter->second.size(), 2);
    for (const auto& module : iter->second) {
        EXPECT_TRUE(module->get_module_name() == "StatusEffectImmune" ||
                    module->get_module_name() == "DamageResistAmount");
    }


    // Add two module owners one with one modules another with 3. 
    // Makes sure that:
    //  The amounts are correct as we add items
    //  Deleting the specific module from the owner with one module clears the owner entry
    //  Deleting a specific module from the owner with three modules, leaves 2
    //  Deleting all from a specific modules clears the owner
    //  And finally that the default assignment remains untouched
    YAML::Node module_data = YAML::LoadFile(k_module_yaml_file);
    EXPECT_TRUE(module_data.IsMap());

    // These are randomly generated and only used for test purposes, if they HAPPEN to overlap with the 
    //  generated healthmanager uuid (HIGHLY unlikely) error out here instead of a more cryptic failure later
    const std::string uuid_owner_1 = "59169d1e-a104-4fea-bf64-0ff7b3d14b91";
    const std::string uuid_owner_2 = "f8cfac32-2f0a-4617-a945-25255b2c928a";
    EXPECT_NE(uuid_owner_1, health_manager->public_test_get_uuid_string());
    EXPECT_NE(uuid_owner_2, health_manager->public_test_get_uuid_string());

    const YAML::Node& test_data_1 = module_data["PoisonImmune"];
    const std::string test_name_1 = test_data_1["name"].as<std::string>();

    const YAML::Node& test_data_2 = module_data["PhysicalResistance"];
    const std::string test_name_2 = test_data_2["name"].as<std::string>();

    const YAML::Node& test_data_3 = module_data["FirePercentResistence"];
    const std::string test_name_3 = test_data_3["name"].as<std::string>();

    const YAML::Node& test_data_4 = module_data["PotionHealBuff"];
    const std::string test_name_4 = test_data_4["name"].as<std::string>();

    // Add the test modules
    health_manager->add_module(uuid_owner_1, test_name_1, test_data_1["data"]);

    EXPECT_EQ(health_manager->public_test_get_health_modules().size(), 2);
    iter = health_manager->public_test_get_health_modules().find(uuid_owner_1);
    EXPECT_TRUE(iter != health_manager->public_test_get_health_modules().cend());
    EXPECT_EQ(iter->second.size(), 1);

    health_manager->add_module(uuid_owner_2, test_name_2, test_data_2["data"]);

    EXPECT_EQ(health_manager->public_test_get_health_modules().size(), 3);
    iter = health_manager->public_test_get_health_modules().find(uuid_owner_2);
    EXPECT_TRUE(iter != health_manager->public_test_get_health_modules().cend());
    EXPECT_EQ(iter->second.size(), 1);

    health_manager->add_module(uuid_owner_2, test_name_3, test_data_3["data"]);
    EXPECT_EQ(health_manager->public_test_get_health_modules().size(), 3);
    EXPECT_EQ(iter->second.size(), 2);

    health_manager->add_module(uuid_owner_2, test_name_4, test_data_4["data"]);
    EXPECT_EQ(health_manager->public_test_get_health_modules().size(), 3);
    EXPECT_EQ(iter->second.size(), 3);

    // Start removing the test modules
    EXPECT_TRUE(health_manager->remove_module(uuid_owner_1, test_name_1));
    EXPECT_EQ(health_manager->public_test_get_health_modules().size(), 2);
    iter = health_manager->public_test_get_health_modules().find(uuid_owner_1);
    EXPECT_TRUE(iter == health_manager->public_test_get_health_modules().cend());
    
    EXPECT_TRUE(health_manager->remove_module(uuid_owner_2, test_name_3));
    EXPECT_EQ(health_manager->public_test_get_health_modules().size(), 2);
    iter = health_manager->public_test_get_health_modules().find(uuid_owner_2);
    EXPECT_TRUE(iter != health_manager->public_test_get_health_modules().cend());
    EXPECT_EQ(iter->second.size(), 2);

    // Check the the remaining modules are #2 and #4
    for (const auto& module : iter->second) {
        EXPECT_TRUE(module->get_module_name() == test_name_2 ||
                    module->get_module_name() == test_name_4);
    }

    EXPECT_TRUE(health_manager->remove_modules_from_owner(uuid_owner_2));
    EXPECT_EQ(health_manager->public_test_get_health_modules().size(), 1);
    iter = health_manager->public_test_get_health_modules().find(health_manager->public_test_get_uuid_string());
    EXPECT_TRUE(iter != health_manager->public_test_get_health_modules().cend());
    EXPECT_EQ(iter->second.size(), 2);
    for (const auto& module : iter->second) {
        EXPECT_TRUE(module->get_module_name() == "StatusEffectImmune" ||
                    module->get_module_name() == "DamageResistAmount");
    }
}

TEST(HealthManager_Test, Test_HealthManager_ApplyHeal_Test) {
    core::MessageSwitchboard switchboard;
    std::shared_ptr<HealthManagerTest> health_manager;

    YAML::Node data_set = YAML::LoadFile(k_yaml_file);
    EXPECT_TRUE(data_set.IsMap());
    health_manager = std::make_shared<HealthManagerTest>();
    EXPECT_TRUE(health_manager->init_manager(switchboard, data_set["TestData_Health_Manager_Apply_Heal"]));

    EXPECT_FLOAT_EQ(health_manager->get_total_health(), 100.f);
    EXPECT_FLOAT_EQ(health_manager->get_health(), 50.f);

    code::client::messages::Heal potion_heal;
    potion_heal.set_heal_type(code::client::messages::Heal::POTION);
    potion_heal.set_amount(10.f);
    code::client::messages::Heal holy_heal;
    holy_heal.set_heal_type(code::client::messages::Heal::HOLY);
    holy_heal.set_amount(10.f);

    std::chrono::milliseconds dt(100);
    // The health manager only processes heal events on the update call.
    health_manager->apply_heal(potion_heal);
    EXPECT_FLOAT_EQ(health_manager->get_health(), 50.f);
    health_manager->update_manager(dt);

    // There is a module that augments the healing effects of potions by 7.5
    EXPECT_FLOAT_EQ(health_manager->get_health(), 67.5f);

    health_manager->apply_heal(holy_heal);
    EXPECT_FLOAT_EQ(health_manager->get_health(), 67.5f);
    health_manager->update_manager(dt);

    // The holy heal should be unscaled
    EXPECT_FLOAT_EQ(health_manager->get_health(), 77.5f);

    // Add both items at once and make sure that the heal effects stop at maximum health
    std::list<code::client::messages::Heal> heal_list;
    heal_list.emplace_back(potion_heal);
    heal_list.emplace_back(holy_heal);

    health_manager->apply_heal(heal_list);
    EXPECT_FLOAT_EQ(health_manager->get_health(), 77.5f);

    health_manager->update_manager(dt);
    EXPECT_FLOAT_EQ(health_manager->get_health(), health_manager->get_total_health());
}

TEST(HealthManager_Test, Test_HealthManager_ApplyDamage_Test) {
    core::MessageSwitchboard switchboard;
    std::shared_ptr<HealthManagerTest> health_manager;

    YAML::Node data_set = YAML::LoadFile(k_yaml_file);
    EXPECT_TRUE(data_set.IsMap());
    health_manager = std::make_shared<HealthManagerTest>();
    EXPECT_TRUE(health_manager->init_manager(switchboard, data_set["TestData_Health_Manager_Apply_Damage"]));

    EXPECT_FLOAT_EQ(health_manager->get_total_health(), 100.f);
    EXPECT_FLOAT_EQ(health_manager->get_health(), 55.f);

    code::client::messages::Damage physical_damage;
    physical_damage.set_damage_type(code::client::messages::Damage::PHYSICAL);
    physical_damage.set_amount(10.f);
    code::client::messages::Damage fire_damage;
    fire_damage.set_damage_type(code::client::messages::Damage::FIRE);
    fire_damage.set_amount(10.f);
    code::client::messages::Damage magic_damage;
    magic_damage.set_damage_type(code::client::messages::Damage::MAGIC);
    magic_damage.set_amount(10.f);
    code::client::messages::Damage physical_fire_damage;
    physical_fire_damage.set_damage_type(code::client::messages::Damage::PHYSICAL | 
                                         code::client::messages::Damage::FIRE);
    physical_fire_damage.set_amount(10.f);

    std::chrono::milliseconds dt(100);
    // The health manager only processes damage events on the update call.
    health_manager->apply_damage(physical_damage);
    EXPECT_FLOAT_EQ(health_manager->get_health(), 55.f);
    health_manager->update_manager(dt);

    // There is a module that reduces the damage effects of physical by 2.5
    EXPECT_FLOAT_EQ(health_manager->get_health(), 47.5f);

    health_manager->apply_damage(fire_damage);
    EXPECT_FLOAT_EQ(health_manager->get_health(), 47.5f);
    health_manager->update_manager(dt);

    // There is a module that reduces the damage effects of fire by 50 %
    EXPECT_FLOAT_EQ(health_manager->get_health(), 42.5f);

    health_manager->apply_damage(magic_damage);
    EXPECT_FLOAT_EQ(health_manager->get_health(), 42.5f);
    health_manager->update_manager(dt);

    // The magic damange should be unscaled
    EXPECT_FLOAT_EQ(health_manager->get_health(), 32.5f);
    
    health_manager->apply_damage(physical_fire_damage);
    EXPECT_FLOAT_EQ(health_manager->get_health(), 32.5f);
    health_manager->update_manager(dt);

    // The physical fire damange should be covered by the partial clauses for 1.4 physical, and 20 % fire
    // 10 - 1.4 = 8.6 * 0.8 = 6.88 applied
    EXPECT_FLOAT_EQ(health_manager->get_health(), 25.62f);

    // Now combine all of them into a list, and make sure the kill conditions are hit.
    std::list<code::client::messages::Damage> damage_list;
    damage_list.emplace_back(physical_damage);
    damage_list.emplace_back(fire_damage);
    damage_list.emplace_back(magic_damage);
    damage_list.emplace_back(physical_fire_damage);

    health_manager->apply_damage(damage_list);
    EXPECT_FLOAT_EQ(health_manager->get_health(), 25.62f);

    health_manager->update_manager(dt);
    EXPECT_FLOAT_EQ(health_manager->get_health(), 0.f);
}

TEST(HealthManager_Test, Test_HealthManager_MessageSubscriber_Test) {
    // Test that the default modules load properly
    core::MessageSwitchboard switchboard;
    std::shared_ptr<HealthManagerTest> health_manager;

    // A simple load of a yaml file and validate the contents
    YAML::Node data_set = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(data_set.IsMap());
    health_manager = std::make_shared<HealthManagerTest>();
    EXPECT_TRUE(health_manager->init_manager(switchboard, data_set["TestData_Health_Manager"]));

    EXPECT_FLOAT_EQ(health_manager->get_total_health(), 100.f);
    EXPECT_FLOAT_EQ(health_manager->get_health(), 75.f);
    
    auto heal_pub = std::make_shared<core::MessagePublisher<Messages::ApplyDirectHeal>>(switchboard);
    auto damage_pub = std::make_shared<core::MessagePublisher<Messages::ApplyDirectDamage>>(switchboard);

    // These are randomly generated and only used for test purposes, if they HAPPEN to overlap with the 
    //  generated healthmanager uuid (HIGHLY unlikely) error out here instead of a more cryptic failure later
    const std::string uuid_owner_1 = "59169d1e-a104-4fea-bf64-0ff7b3d14b91";
    const std::string uuid_owner_2 = "f8cfac32-2f0a-4617-a945-25255b2c928a";
    EXPECT_NE(uuid_owner_1, health_manager->public_test_get_uuid_string());
    EXPECT_NE(uuid_owner_2, health_manager->public_test_get_uuid_string());

    std::chrono::milliseconds dt(100);
    // publish heal and damage events and check that the health is updated as expected.
    Messages::ApplyDirectHeal heal1 {5.f, code::client::messages::Heal::POTION, uuid_owner_1};
    Messages::ApplyDirectHeal heal2 {30.f, code::client::messages::Heal::HOLY, uuid_owner_2};
    Messages::ApplyDirectDamage damage1 {30.f, code::client::messages::Damage::PHYSICAL, uuid_owner_1};
    Messages::ApplyDirectDamage damage2 {50.f, code::client::messages::Damage::FIRE, uuid_owner_2};

    bool character_dead = false;
    auto death_subscriber = std::make_shared<core::MessageSubscriber<Messages::OnDeath>>(switchboard);
    death_subscriber->register_callback([&character_dead](const Messages::OnDeath& death_msg) {
        character_dead = true;
    });
    EXPECT_FALSE(character_dead);

    switchboard.publish_message(heal1);
    EXPECT_FLOAT_EQ(health_manager->get_health(), 75.f);
    health_manager->update_manager(dt);
    EXPECT_FLOAT_EQ(health_manager->get_health(), 80.f);

    switchboard.publish_message(damage1);
    EXPECT_FLOAT_EQ(health_manager->get_health(), 80.f);
    health_manager->update_manager(dt);
    EXPECT_FLOAT_EQ(health_manager->get_health(), 50.f);
    
    // queue up enough damage to kill, and the heals. If all come in on the same frame, 
    //  the heals should take presidence
    switchboard.publish_message(damage1);   // 50 -30 = 20
    switchboard.publish_message(damage2);   // 20 -50 = -30
    switchboard.publish_message(heal1);     // 5 heal
    switchboard.publish_message(heal2);     // 30 heal

    EXPECT_FLOAT_EQ(health_manager->get_health(), 50.f);
    health_manager->update_manager(dt);
    EXPECT_FLOAT_EQ(health_manager->get_health(), 5.f);
    EXPECT_FALSE(character_dead);

    // now kill the character, and check that subsequent heals don't do anything
    switchboard.publish_message(damage2);
    EXPECT_FLOAT_EQ(health_manager->get_health(), 5.f);
    health_manager->update_manager(dt);
    EXPECT_FLOAT_EQ(health_manager->get_health(), 0.f);
    EXPECT_TRUE(character_dead);

    switchboard.publish_message(heal2);
    EXPECT_FLOAT_EQ(health_manager->get_health(), 0.f);
    health_manager->update_manager(dt);
    EXPECT_FLOAT_EQ(health_manager->get_health(), 0.f);
}

TEST(HealthManager_Test, Test_HealthManager_MessagePublisher_Test) {
    Status::StatusEffectLibrary::get_Instance()->init_from_file(k_yaml_library_file);

    // Test that the default modules load properly
    core::MessageSwitchboard switchboard;
    std::shared_ptr<HealthManagerTest> health_manager;

    // A simple load of a yaml file and validate the contents
    YAML::Node data_set = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(data_set.IsMap());
    health_manager = std::make_shared<HealthManagerTest>();
    EXPECT_TRUE(health_manager->init_manager(switchboard, data_set["TestData_Health_Manager"]));

    EXPECT_FLOAT_EQ(health_manager->get_total_health(), 100.f);
    EXPECT_FLOAT_EQ(health_manager->get_health(), 75.f);

    // The health manager should broadcast when a status effect is applied (i.e. through an attack)
    //  and when the character dies
    
    // death subscriber
    bool character_dead = false;
    auto death_subscriber = std::make_shared<core::MessageSubscriber<Messages::OnDeath>>(switchboard);
    death_subscriber->register_callback([&character_dead](const Messages::OnDeath& death_msg) {
        character_dead = true;
    });
    EXPECT_FALSE(character_dead);

    // health change subscriber
    float health_change = 0.f;
    float current_health = 0.f;
    std::string uuid = "";
    auto health_change_subscriber = std::make_shared<core::MessageSubscriber<Messages::OnHealthChange>>(switchboard);
    health_change_subscriber->register_callback([&](const Messages::OnHealthChange& health_msg) {
        health_change = health_msg.amount_change;
        current_health = health_msg.current_health;
        uuid = health_msg.health_manager_uuid;
    });
    EXPECT_FLOAT_EQ(health_change, 0.f);
    EXPECT_FLOAT_EQ(current_health, 0.f);
    EXPECT_EQ(uuid, "");

    // status subscriber
    std::string status_name = "";
    auto status_subscriber = std::make_shared<core::MessageSubscriber<Messages::ApplyStatus>>(switchboard);
    status_subscriber->register_callback([&status_name](const Messages::ApplyStatus& status_msg) {
        status_name = status_msg.status_effect_name;
    });
    EXPECT_EQ(status_name, "");

    // Set up the damage and heal events to test
    std::chrono::milliseconds dt(100);

    // Test the health change and status publish
    code::client::messages::Damage magic_damage;
    magic_damage.set_damage_type(code::client::messages::Damage::MAGIC);
    magic_damage.set_amount(10.f);
    magic_damage.add_status_effect("Ignite_V1");

    // Apply the magic damage which will trigger the health change and status messages
    health_manager->apply_damage(magic_damage);
    EXPECT_FLOAT_EQ(health_manager->get_health(), 75.f);
    health_manager->update_manager(dt);

    EXPECT_FLOAT_EQ(health_manager->get_health(), 65.f);
    EXPECT_FLOAT_EQ(health_change, -10.f);
    EXPECT_FLOAT_EQ(current_health, health_manager->get_health());
    EXPECT_EQ(uuid, health_manager->public_test_get_uuid_string());

    EXPECT_EQ(status_name, "Ignite_V1");
    EXPECT_FALSE(character_dead);

    // Reset The Test Values
    character_dead = false;
    health_change = 0.f;
    current_health = 0.f;
    uuid = "";
    status_name = "";

    // Test that the heal causes a health change
    code::client::messages::Heal holy_heal;
    holy_heal.set_heal_type(code::client::messages::Heal::HOLY);
    holy_heal.set_amount(10.f);

    health_manager->apply_heal(holy_heal);
    EXPECT_FLOAT_EQ(health_manager->get_health(), 65.f);
    health_manager->update_manager(dt);

    EXPECT_FLOAT_EQ(health_manager->get_health(), 75.f);
    EXPECT_FLOAT_EQ(health_change, 10.f);
    EXPECT_FLOAT_EQ(current_health, health_manager->get_health());
    EXPECT_EQ(uuid, health_manager->public_test_get_uuid_string());

    EXPECT_EQ(status_name, "");
    EXPECT_FALSE(character_dead);

    // Reset The Test Values
    character_dead = false;
    health_change = 0.f;
    current_health = 0.f;
    uuid = "";
    status_name = "";

    // Kill the character and check for death
    const std::string uuid_owner_1 = "59169d1e-a104-4fea-bf64-0ff7b3d14b91";
    Messages::ApplyDirectDamage damage1 {80.f, code::client::messages::Damage::PHYSICAL, uuid_owner_1};

    switchboard.publish_message(damage1);
    EXPECT_FLOAT_EQ(health_manager->get_health(), 75.f);
    health_manager->update_manager(dt);

    EXPECT_FLOAT_EQ(health_manager->get_health(), 0.f);
    EXPECT_FLOAT_EQ(health_change, -75.f);
    EXPECT_FLOAT_EQ(current_health, health_manager->get_health());
    EXPECT_EQ(uuid, health_manager->public_test_get_uuid_string());

    EXPECT_EQ(status_name, "");
    EXPECT_TRUE(character_dead);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
