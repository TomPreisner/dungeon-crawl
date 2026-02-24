load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "state_machine_test",
    srcs = ["state_machine_test.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "//code/client/core/state_machine:state_machine",
    ],
    size="small"
)