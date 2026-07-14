load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "character_base",
    srcs = ["character_base.cpp"],
    hdrs = ["character_base.h"],
    deps = [
        "@behaviortree_cpp//:bt_cpp",
        "//code/client/health:health_manager",
        "//code/client/object:object_dynamic",
        "//code/client/status_effect:status_effect_manager",
        "//code/core/message_system:message_system",
    ],
    visibility = ["//visibility:public"],
)
