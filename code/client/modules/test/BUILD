load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "module_factory_test",
    srcs = ["module_factory_test.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "@yaml-cpp",
        "//code/client/modules:module_factory",
        "//code/client/status_effect:status_effect_library",
    ],
    data = [
        "//code/client/modules/test/data:test_module_factory_data_yaml",
        "//code/client/modules/test/data:test_status_effect_library_data_yaml"
    ],
    size="small"
)
