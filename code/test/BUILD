load("@rules_cc//cc:defs.bzl", "cc_test")

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

cc_test(
    name = "protobuf_test",
    srcs = ["protobuf_test.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "@protobuf//:protobuf",
        "@protobuf//src/google/protobuf/util:time_util",
        "//code/test/test_data:test_data_cc_proto",
    ],
)

cc_test(
    name = "ogre_test",
    srcs = ["ogre_test.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "@ogre_sdk//:ogre",
    ],
    data = ["//code/test/test_data:test_data_ogre"],
)
