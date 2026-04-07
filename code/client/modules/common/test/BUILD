load("@rules_cc//cc:defs.bzl", "cc_test")


cc_test(
    name = "status_effect_test",
    srcs = ["status_effect_test.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "@yaml-cpp",
        "//code/client/status_effect:status_effect",
    ],
    data = ["//code/client/status_effect/test/data:test_status_effect_data_yaml"],
    size="small"
)
