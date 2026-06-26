load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "damage_type_amount_descriptor_test",
    srcs = ["damage_type_amount_descriptor_test.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "@yaml-cpp",
        "//code/client/messages/proto:damage_cc_proto",
        "//code/client/modules/descriptors:damage_type_amount_descriptor",
    ],
    data = ["//code/client/modules/descriptors/test/data:test_damage_type_descriptor_yaml"],
    size="small"
)

cc_test(
    name = "damage_type_percent_descriptor_test",
    srcs = ["damage_type_percent_descriptor_test.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "@yaml-cpp",
        "//code/client/messages/proto:damage_cc_proto",
        "//code/client/modules/descriptors:damage_type_percent_descriptor",
    ],
    data = ["//code/client/modules/descriptors/test/data:test_damage_type_descriptor_yaml"],
    size="small"
)

cc_test(
    name = "heal_type_amount_descriptor_test",
    srcs = ["heal_type_amount_descriptor_test.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "@yaml-cpp",
        "//code/client/modules/descriptors:heal_type_amount_descriptor",
    ],
    data = ["//code/client/modules/descriptors/test/data:test_heal_type_descriptor_yaml"],
    size="small"
)

cc_test(
    name = "status_effect_descriptor_test",
    srcs = ["status_effect_descriptor_test.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "@yaml-cpp",
        "//code/client/status_effect:status_effect",
        "//code/client/status_effect:status_effect_library",
        "//code/client/modules/descriptors:status_effect_descriptor",
    ],
    data = [
        "//code/client/modules/descriptors/test/data:test_status_effect_descriptor_yaml",
        "//code/client/modules/descriptors/test/data:test_status_effect_library_data_yaml"
    ],
    size="small"
)
