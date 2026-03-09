load("@rules_cc//cc:defs.bzl", "cc_test")
load("@rules_python//python:defs.bzl", "py_test")
load("@pip//:requirements.bzl", "requirement")

cc_test(
    name = "behaviortree_cpp_test",
    srcs = ["behaviorTreeCPP_test.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "@behaviortree_cpp//:bt_cpp",
    ],
    data = ["//code/test/test_data:test_data_behaviortree_cpp"],
    size="small"
)

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
    name = "ogre_test",
    srcs = ["ogre_test.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "@ogre_sdk//:ogre",
    ],
    data = ["//code/test/test_data:test_data_ogre"],
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
    size="small"
)

cc_test(
    name = "spdlog_test",
    srcs = ["spdlog_test.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "@spdlog",
    ],
    size="small"
)

cc_test(
    name = "yaml_cpp_test",
    srcs = ["yaml_cpp_test.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "@yaml-cpp",
    ],
    data = ["//code/test/test_data:test_data_yaml"],
    size="small"
)

py_test(
    name = "py_navmesh_test",
    srcs = ["py_navmesh_test.py"],
    deps = [
        requirement("pytest"),
        requirement("pynavmesh"),
    ],
    data = ["//code/test/test_data:test_data_navmesh"],
    size="medium"
)