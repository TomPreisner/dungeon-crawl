load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "effect_type_test",
    srcs = ["effect_type_test.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "//code/client/status_effect/effects:effect_type",
    ],
    size="small"
)

cc_test(
    name = "effect_data_test",
    srcs = ["effect_data_test.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "//code/client/messages/proto:damage_cc_proto",
        "//code/client/messages/proto:heal_cc_proto",
        "//code/client/status_effect/effects:effect_data",
    ],
    size="small"
)

cc_test(
    name = "effect_factory_test",
    srcs = ["effect_factory_test.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "//code/client/status_effect/effects:effect_base",
        "//code/client/status_effect/effects:effect_factory",
        "//code/client/status_effect/effects/types:effect_clear",
        "//code/client/status_effect/effects/types:effect_damage",
        "//code/client/status_effect/effects/types:effect_damage_multiply",
        "//code/client/status_effect/effects/types:effect_heal",
        "//code/client/status_effect/effects/types:effect_heal_multiply",
    ],
    data = ["//code/client/status_effect/effects/test/data:test_effect_factory_yaml"],
    size="small"
)
