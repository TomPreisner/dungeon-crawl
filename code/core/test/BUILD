load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "log_manager_test",
    srcs = ["log_manager_test.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "//code/core:log_manager",
    ],
    size="small"
)

cc_test(
    name = "state_machine_test",
    srcs = ["state_machine_test.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "//code/core:state_machine",
    ],
    size="small"
)
