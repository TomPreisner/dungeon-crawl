load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "message_system_test",
    srcs = ["message_system_test.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "//code/core/message_system:message_system",
        "//code/core/message_system/test/data:message_system_test_cc_proto",        
    ],
    size="small"
)
