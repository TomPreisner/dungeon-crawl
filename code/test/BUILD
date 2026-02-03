load("@rules_cc//cc:defs.bzl", "cc_test")
load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_test(
    name = "eigen_test",
    srcs = ["eigen_test.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "@eigen",
    ],
    size="small"
)
