load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "health_messages",
    hdrs = ["health_messages.h"],
    deps = ["//code/client/messages/proto:heal_cc_proto"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "status_messages",
    hdrs = ["status_messages.h"],
    visibility = ["//visibility:public"],
)
