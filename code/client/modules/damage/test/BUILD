load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "damage_type_apply_module_test",
    srcs = ["damage_type_apply_module_test.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "@yaml-cpp",
        "//code/client/messages/proto:damage_cc_proto",
        "//code/client/modules/damage:damage_type_apply_module",
    ],
    data = ["//code/client/modules/damage/test/data:test_damage_type_apply_module_yaml"],
    size="small"
)

cc_test(
    name = "status_effect_apply_module_test",
    srcs = ["status_effect_apply_module_test.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "@yaml-cpp",
        "//code/client/status_effect:status_effect",
        "//code/client/status_effect:status_effect_library",
        "//code/client/modules/damage:status_effect_apply_module",
    ],
    data = [
        "//code/client/modules/damage/test/data:test_status_effect_apply_module_yaml",
        "//code/client/modules/damage/test/data:test_status_effect_library_data_yaml"
    ],
    size="small"
)
