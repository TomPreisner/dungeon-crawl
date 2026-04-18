load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "log_manager",
    hdrs = ["log_manager.h"],
    deps = ["@spdlog"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "message_dispatch",
    hdrs = ["message_dispatch.h"],
    deps = ["@boost.uuid"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "state_machine",
    hdrs = ["state_machine.h"],
    visibility = ["//visibility:public"],
)
