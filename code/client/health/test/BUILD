load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "health_manager_test",
    srcs = ["health_manager_test.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "@yaml-cpp",
        "//code/client/health:health_manager",
        "//code/client/modules/common:base_module",
        "//code/client/modules/health:status_effect_immune_module",
        "//code/core/message_system:message_system",
    ],
    data = [
        "//code/client/health/test/data:test_health_manager_data_yaml",
    ],
    size="small"
)
