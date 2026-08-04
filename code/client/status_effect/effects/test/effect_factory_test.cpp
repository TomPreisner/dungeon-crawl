#include <gtest/gtest.h>

#include <iostream>
#include <stdint.h>
#include <string>
#include "code/client/status_effect/effects/base_effect.h"
#include "code/client/status_effect/effects/effect_factory.h"
#include "code/client/status_effect/effects/types/effect_clear.h"
#include "code/client/status_effect/effects/types/effect_damage.h"
#include "code/client/status_effect/effects/types/effect_damage_multiply.h"
#include "code/client/status_effect/effects/types/effect_heal.h"
#include "code/client/status_effect/effects/types/effect_heal_multiply.h"

std::string k_yaml_file = "code\\client\\status_effect\\effects\\test\\data\\test_effect_factory.yaml";

TEST(Effect_Type, Test_EffectData) {
    // A simple load of a yaml file and validate the contents
    YAML::Node config = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(config.IsMap());

    YAML::Node effect_node = config["TestData_Create"];
    EXPECT_TRUE(effect_node);
    EXPECT_TRUE(effect_node.IsSequence());

    EXPECT_EQ(7, effect_node.size());

    // Test that the effect factory creates the appropriate types

    std::shared_ptr<Status::Effect_Base> test = Status::EffectFactory::get_Instance()->create_effect(effect_node[0]);
    EXPECT_EQ(test.get(), nullptr);
    
    test = Status::EffectFactory::get_Instance()->create_effect(effect_node[1]);
    EXPECT_EQ(test.get(), nullptr);

    test = Status::EffectFactory::get_Instance()->create_effect(effect_node[2]);
    EXPECT_NE(dynamic_cast<Status::Effect_Clear*>(test.get()), nullptr);

    test = Status::EffectFactory::get_Instance()->create_effect(effect_node[3]);
    EXPECT_NE(dynamic_cast<Status::Effect_Damage*>(test.get()), nullptr);

    test = Status::EffectFactory::get_Instance()->create_effect(effect_node[4]);
    EXPECT_NE(dynamic_cast<Status::Effect_Damage_Multiply*>(test.get()), nullptr);

    test = Status::EffectFactory::get_Instance()->create_effect(effect_node[5]);
    EXPECT_NE(dynamic_cast<Status::Effect_Heal*>(test.get()), nullptr);

    test = Status::EffectFactory::get_Instance()->create_effect(effect_node[6]);
    EXPECT_NE(dynamic_cast<Status::Effect_Heal_Multiply*>(test.get()), nullptr);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
