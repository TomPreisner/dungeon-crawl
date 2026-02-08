load("@protobuf//bazel:proto_library.bzl", "proto_library")
load("@protobuf//bazel:cc_proto_library.bzl", "cc_proto_library")

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
