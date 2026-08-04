#include <gtest/gtest.h>

#include <iostream>
#include <stdint.h>
#include <string>
#include "code/client/status_effect/effects/effect_type.h"

TEST(Effect_Type, Test_EffectTypeToString) {
    // Test that all effects get converted and that the strings don't overlap
    const uint8_t count = static_cast<uint8_t>(Status::EffectType::COUNT);

    std::vector<std::string> enum_name_list;
    enum_name_list.reserve(count);

    for (uint8_t i = 0; i < count; ++i) {
        std::string enum_name = Status::to_string(static_cast<Status::EffectType>(i));

        for (uint8_t j = 0; j < enum_name_list.size(); ++j) {
            EXPECT_NE(enum_name, enum_name_list[j]);
        }
        enum_name_list.push_back(enum_name);
    }
    EXPECT_EQ(enum_name_list.size(), count);
}

TEST(Effect_Type, Test_StringToEffectType) {
    // Test that the expected strings convert to the expected types
    EXPECT_EQ(Status::convert_to_effect_type("CLEAR"), Status::EffectType::CLEAR);
    EXPECT_EQ(Status::convert_to_effect_type("DAMAGE"), Status::EffectType::DAMAGE);
    EXPECT_EQ(Status::convert_to_effect_type("DAMAGE_MULTIPLY"), Status::EffectType::DAMAGE_MULTIPLY);
    EXPECT_EQ(Status::convert_to_effect_type("HEAL"), Status::EffectType::HEAL);
    EXPECT_EQ(Status::convert_to_effect_type("HEAL_MULTIPLY"), Status::EffectType::HEAL_MULTIPLY);
}

TEST(Effect_Type, Test_EffectBackAndForth) {
    // convert from enum to string an back again and it converts things properly
    const uint8_t count = static_cast<uint8_t>(Status::EffectType::COUNT);

    std::vector<std::string> enum_name_list;
    enum_name_list.reserve(count);

    for (uint8_t i = 0; i < count; ++i) {
        std::string enum_name = Status::to_string(static_cast<Status::EffectType>(i));

        EXPECT_EQ(static_cast<Status::EffectType>(i), Status::convert_to_effect_type(enum_name));
    }
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
