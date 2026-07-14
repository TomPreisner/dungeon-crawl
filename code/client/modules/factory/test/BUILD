load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "health_module_factory_test",
    srcs = ["health_module_factory_test.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "@yaml-cpp",
        "//code/client/modules/factory:health_module_factory",
        "//code/client/status_effect:status_effect_library",
    ],
    data = [
        "//code/client/modules/factory/test/data:test_health_module_factory_data_yaml",
        "//code/client/modules/factory/test/data:test_status_effect_library_data_yaml"
    ],
    size="small"
)
