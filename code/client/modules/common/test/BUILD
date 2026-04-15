load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "damage_type_module_common_test",
    srcs = ["damage_type_module_common_test.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "@yaml-cpp",
        "//code/client/damage/proto:damage_cc_proto",
        "//code/client/modules/common:damage_type_module_common",
    ],
    data = ["//code/client/modules/common/test/data:test_damage_type_module_common_yaml"],
    size="small"
)

cc_test(
    name = "heal_type_module_common_test",
    srcs = ["heal_type_module_common_test.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "@yaml-cpp",
        "//code/client/health/proto:heal_cc_proto",
        "//code/client/modules/common:heal_type_module_common",
    ],
    data = ["//code/client/modules/common/test/data:test_heal_type_module_common_yaml"],
    size="small"
)

cc_test(
    name = "status_effect_module_common_test",
    srcs = ["status_effect_module_common_test.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "@yaml-cpp",
        "//code/client/status_effect:status_effect",
        "//code/client/status_effect:status_effect_library",
        "//code/client/modules/common:status_effect_module_common",
    ],
    data = [
        "//code/client/modules/common/test/data:test_status_effect_module_common_yaml",
        "//code/client/modules/common/test/data:test_status_effect_library_data_yaml"
    ],
    size="small"
)
