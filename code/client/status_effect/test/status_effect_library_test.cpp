#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include "code/client/status_effect/status_effect_library.h"

std::string k_yaml_file = "code\\client\\status_effect\\test\\data\\test_status_effect_library_data.yaml";

TEST(StatusEffectLibrary, Test_StatusEffectLibraryInit) {
    // A simple load of a yaml file and validate the contents
    {
        YAML::Node config = YAML::LoadFile(k_yaml_file);
        EXPECT_TRUE(config.IsMap());
    }

    Status::StatusEffectLibrary::get_Instance()->init_from_file(k_yaml_file);

    {
        EXPECT_TRUE(Status::StatusEffectLibrary::get_Instance()->has_status_effect("Ignite_V1"));
        auto effect = Status::StatusEffectLibrary::get_Instance()->get_status_effect("Ignite_V1");
        EXPECT_TRUE(effect.has_value());
        EXPECT_EQ(effect->get_type(), Status::StatusEffectType::FIRE);
        EXPECT_EQ(effect->get_icon(), "flame_icon_small.png");
        EXPECT_EQ(effect->get_vfx(), "fire_effect.vfx");
        EXPECT_EQ(effect->get_sfx(), "fire_crackle.wav");
    }
    {
        EXPECT_TRUE(Status::StatusEffectLibrary::get_Instance()->has_status_effect("DeepCut_V1"));
        auto effect = Status::StatusEffectLibrary::get_Instance()->get_status_effect("DeepCut_V1");
        EXPECT_TRUE(effect.has_value());
        EXPECT_EQ(effect->get_type(), Status::StatusEffectType::BLEED);
        EXPECT_EQ(effect->get_icon(), "true");
        EXPECT_EQ(effect->get_vfx(), "15");
        EXPECT_EQ(effect->get_sfx(), "12");
    }
    {
        EXPECT_TRUE(Status::StatusEffectLibrary::get_Instance()->has_status_effect("Venom_V1"));
        auto effect = Status::StatusEffectLibrary::get_Instance()->get_status_effect("Venom_V1");
        EXPECT_TRUE(effect.has_value());
        EXPECT_EQ(effect->get_type(), Status::StatusEffectType::POISON);
        EXPECT_EQ(effect->get_icon(), "");
        EXPECT_EQ(effect->get_vfx(), "");
        EXPECT_EQ(effect->get_sfx(), "");
    }

    {
        EXPECT_FALSE(Status::StatusEffectLibrary::get_Instance()->has_status_effect(""));
        auto effect = Status::StatusEffectLibrary::get_Instance()->get_status_effect("");
        EXPECT_FALSE(effect.has_value());
    }
    {
        EXPECT_FALSE(Status::StatusEffectLibrary::get_Instance()->has_status_effect("FooBar"));
        auto effect = Status::StatusEffectLibrary::get_Instance()->get_status_effect("FooBar");
        EXPECT_FALSE(effect.has_value());
    }

    Status::StatusEffectLibrary::get_Instance()->shutdown();
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
