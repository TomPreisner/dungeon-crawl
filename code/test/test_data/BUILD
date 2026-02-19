load("@protobuf//bazel:proto_library.bzl", "proto_library")
load("@protobuf//bazel:cc_proto_library.bzl", "cc_proto_library")

filegroup(
    name = "test_data_ogre",
    srcs = [
        "plugins.cfg",
        "resources.cfg",
    ],
    visibility = ["//visibility:public"],
)

proto_library(
    name = "test_data_proto",
    srcs = ["test_data.proto"],
    deps = ["@protobuf//:timestamp_proto"],
)

cc_proto_library(
    name = "test_data_cc_proto",
    deps = [":test_data_proto"],
    visibility = ["//visibility:public"],
)
