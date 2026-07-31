load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "effect_clear_test",
    srcs = ["effect_clear_test.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "@yaml-cpp",
        "//code/client/status_effect/effects:effect_clear",
        "//code/client/status_effect/effects:effect_type",
    ],
    data = ["//code/client/status_effect/effects/test/data:test_effect_clear_data_yaml"],
    size="small"
)

cc_test(
    name = "effect_damage_test",
    srcs = ["effect_damage_test.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "@yaml-cpp",
        "//code/client/messages/proto:damage_cc_proto",
        "//code/client/status_effect/effects:effect_damage",
        "//code/client/status_effect/effects:effect_type",
    ],
    data = ["//code/client/status_effect/effects/test/data:test_effect_damage_data_yaml"],
    size="small"
)

cc_test(
    name = "effect_damage_multiply_test",
    srcs = ["effect_damage_multiply_test.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "@yaml-cpp",
        "//code/client/messages/proto:damage_cc_proto",
        "//code/client/status_effect/effects:effect_damage_multiply",
        "//code/client/status_effect/effects:effect_type",
    ],
    data = ["//code/client/status_effect/effects/test/data:test_effect_damage_multiply_data_yaml"],
    size="small"
)

cc_test(
    name = "effect_heal_test",
    srcs = ["effect_heal_test.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "@yaml-cpp",
        "//code/client/messages/proto:heal_cc_proto",
        "//code/client/status_effect/effects:effect_heal",
        "//code/client/status_effect/effects:effect_type",
    ],
    data = ["//code/client/status_effect/effects/test/data:test_effect_heal_data_yaml"],
    size="small"
)

cc_test(
    name = "effect_heal_multiply_test",
    srcs = ["effect_heal_multiply_test.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "@yaml-cpp",
        "//code/client/messages/proto:heal_cc_proto",
        "//code/client/status_effect/effects:effect_heal_multiply",
        "//code/client/status_effect/effects:effect_type",
    ],
    data = ["//code/client/status_effect/effects/test/data:test_effect_heal_multiply_data_yaml"],
    size="small"
)
