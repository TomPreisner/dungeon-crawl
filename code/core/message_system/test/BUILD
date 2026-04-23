load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "message_dispatch_test",
    srcs = ["message_dispatch_test.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "//code/core/message_system:message_dispatch",
        "//code/core/message_system/test/data:message_test_data",
        "//code/core/message_system/test/data:message_system_test_cc_proto",
    ],
    size="small"
)

cc_test(
    name = "message_publisher_test",
    srcs = ["message_publisher_test.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "//code/core/message_system:message_system",
        "//code/core/message_system/test/data:message_test_data",
    ],
    size="small"
)

cc_test(
    name = "message_subscriber_test",
    srcs = ["message_subscriber_test.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "//code/core/message_system:message_system",
        "//code/core/message_system/test/data:message_test_data",
    ],
    size="small"
)

cc_test(
    name = "message_switchboard_test",
    srcs = ["message_switchboard_test.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "//code/core/message_system:message_dispatch",
        "//code/core/message_system:message_system",
        "//code/core/message_system/test/data:message_test_data",
    ],
    size="small"
)

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

cc_test(
    name = "message_system_example_test",
    srcs = ["message_system_example_test.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "//code/core/message_system:message_system",
        "//code/core/message_system/test/example:message_example",
    ],
    size="small"
)

