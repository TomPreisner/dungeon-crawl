load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "test_effect",
    srcs = ["test_effect.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "@yaml-cpp",
        "//code/client/status_effect:effect",
        "//code/core:log_manager",
    ],
    data = ["//code/client/status_effect/test/data:test_effect_data_yaml"],
    size="small"
)

