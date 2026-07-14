load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "effect",
    srcs = ["effect.cpp"],
    hdrs = ["effect.h"],
    deps = [
        "@yaml-cpp",
        "//code/core:log_manager",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "status_effect",
    srcs = ["status_effect.cpp"],
    hdrs = [
        "status_effect.h",
        "status_effect_callback_interface.h"
    ],
    deps = [
        "@boost.uuid",
        "@yaml-cpp",
        ":effect",
        "//code/core:log_manager",
        "//code/core:state_machine",
        "//code/client/messages/proto:damage_cc_proto",
        "//code/client/messages/proto:heal_cc_proto",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "status_effect_library",
    srcs = ["status_effect_library.cpp"],
    hdrs = ["status_effect_library.h"],
    deps = [
        "@yaml-cpp",
        ":status_effect",
        "//code/core:log_manager",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "status_effect_manager",
    srcs = ["status_effect_manager.cpp"],
    hdrs = ["status_effect_manager.h"],
    deps = [
        "@yaml-cpp",
        ":status_effect",
        ":status_effect_library",
        "//code/client/messages:health_messages",
        "//code/client/messages:status_messages",
        "//code/core:log_manager",
        "//code/core/message_system:message_system",
    ],
    visibility = ["//visibility:public"],
)
