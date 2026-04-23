load("@rules_cc//cc:cc_library.bzl", "cc_library")
load("@protobuf//bazel:proto_library.bzl", "proto_library")
load("@protobuf//bazel:cc_proto_library.bzl", "cc_proto_library")

cc_library(
    name = "message_test_data",
    hdrs = ["message_test_data.h"],
    visibility = ["//visibility:public"],
)

proto_library(
    name = "message_system_test_proto",
    srcs = ["message_system_test.proto"],
    deps = ["@protobuf//:timestamp_proto"],
)

cc_proto_library(
    name = "message_system_test_cc_proto",
    deps = [":message_system_test_proto"],
    visibility = ["//visibility:public"],
)
