load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "health_manager",
    srcs = ["health_manager.cpp"],
    hdrs = ["health_manager.h"],
    deps = [
        "@boost.uuid",
        "@yaml-cpp",
        "//code/client/messages:health_messages",
        "//code/client/messages:status_messages",
        "//code/client/messages/proto:damage_cc_proto",
        "//code/client/messages/proto:heal_cc_proto",
        "//code/client/modules/common:base_module",
        "//code/client/modules/health:health_module",
        "//code/client/modules:module_factory",
        "//code/core:log_manager",
        "//code/core/message_system:message_system",
    ],
    visibility = ["//visibility:public"],
)
