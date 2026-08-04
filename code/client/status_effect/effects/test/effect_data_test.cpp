#include <gtest/gtest.h>

#include <iostream>
#include <stdint.h>
#include <string>
#include "code/client/status_effect/effects/effect_data.h"
#include "code/client/messages/proto/damage.pb.h"
#include "code/client/messages/proto/heal.pb.h"

TEST(Effect_Type, Test_EffectData) {
    Status::EffectData testData(5.f);
    EXPECT_FLOAT_EQ(testData.amount, 5.f);

    Status::EffectData_Damage_Type testDamageData(3.7f, code::client::messages::Damage::PHYSICAL);
    EXPECT_FLOAT_EQ(testDamageData.amount, 3.7f);
    EXPECT_EQ(testDamageData.damage_type, code::client::messages::Damage::PHYSICAL);

    Status::EffectData_Heal_Type testHealData(2.1f, code::client::messages::Heal::POTION);
    EXPECT_FLOAT_EQ(testHealData.amount, 2.1f);
    EXPECT_EQ(testHealData.heal_type, code::client::messages::Heal::POTION);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
