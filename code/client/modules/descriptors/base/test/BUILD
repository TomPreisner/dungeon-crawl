load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "damage_type_descriptor_base_test",
    srcs = ["damage_type_descriptor_base_test.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "@yaml-cpp",
        "//code/client/messages/proto:damage_cc_proto",
        "//code/client/modules/descriptors/base:damage_type_descriptor_base",
    ],
    data = ["//code/client/modules/descriptors/base/test/data:test_damage_type_descriptor_base_yaml"],
    size="small"
)

cc_test(
    name = "heal_type_descriptor_base_test",
    srcs = ["heal_type_descriptor_base_test.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "@yaml-cpp",
        "//code/client/messages/proto:heal_cc_proto",
        "//code/client/modules/descriptors/base:heal_type_descriptor_base",
    ],
    data = ["//code/client/modules/descriptors/base/test/data:test_heal_type_descriptor_base_yaml"],
    size="small"
)

cc_test(
    name = "status_effect_descriptor_base_test",
    srcs = ["status_effect_descriptor_base_test.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "@yaml-cpp",
        "//code/client/status_effect:status_effect",
        "//code/client/status_effect:status_effect_library",
        "//code/client/modules/descriptors/base:status_effect_descriptor_base",
    ],
    data = [
        "//code/client/modules/descriptors/base/test/data:test_status_effect_descriptor_base_yaml",
        "//code/client/modules/descriptors/base/test/data:test_status_effect_library_data_yaml"
    ],
    size="small"
)
