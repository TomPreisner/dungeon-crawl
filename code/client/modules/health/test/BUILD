load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "damage_type_resist_health_module_test",
    srcs = ["damage_type_resist_health_module_test.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "@yaml-cpp",
        "//code/client/messages/proto:damage_cc_proto",
        "//code/client/messages/proto:heal_cc_proto",
        "//code/client/modules/health:damage_type_resist_health_module",
    ],
    data = ["//code/client/modules/health/test/data:test_damage_type_resist_module_yaml"],
    size="small"
)

cc_test(
    name = "damage_type_resist_percent_health_module_test",
    srcs = ["damage_type_resist_percent_health_module_test.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "@yaml-cpp",
        "//code/client/messages/proto:damage_cc_proto",
        "//code/client/messages/proto:heal_cc_proto",
        "//code/client/modules/health:damage_type_resist_percent_health_module",
    ],
    data = ["//code/client/modules/health/test/data:test_damage_type_resist_module_yaml"],
    size="small"
)

cc_test(
    name = "heal_type_increase_health_module_test",
    srcs = ["heal_type_increase_health_module_test.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "@yaml-cpp",
        "//code/client/messages/proto:damage_cc_proto",
        "//code/client/messages/proto:heal_cc_proto",
        "//code/client/modules/health:heal_type_increase_health_module",
    ],
    data = ["//code/client/modules/health/test/data:test_heal_type_increase_module_yaml"],
    size="small"
)

cc_test(
    name = "status_effect_immune_health_module_test",
    srcs = ["status_effect_immune_health_module_test.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "@yaml-cpp",
        "//code/client/messages/proto:damage_cc_proto",
        "//code/client/messages/proto:heal_cc_proto",
        "//code/client/status_effect:status_effect",
        "//code/client/status_effect:status_effect_library",
        "//code/client/modules/health:status_effect_immune_health_module",
    ],
    data = [
        "//code/client/modules/health/test/data:test_status_effect_immune_module_yaml",
        "//code/client/modules/health/test/data:test_status_effect_library_data_yaml"
    ],
    size="small"
)
