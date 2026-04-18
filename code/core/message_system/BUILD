load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "message_dispatch_base",
    hdrs = ["message_dispatch_base.h"],
    # visibility hidden on purpose
)

cc_library(
    name = "message_dispatch",
    hdrs = ["message_dispatch.h"],
    deps = [
        "@boost.uuid",
        ":message_dispatch_base",
    ]
    # visibility hidden on purpose
)

cc_library(
    name = "message_system",
    hdrs = [
        "message_publisher.h",
        "message_subscriber.h",
        "message_switchboard.h"
    ],
    deps = [
        ":message_dispatch",
        "//code/core:log_manager",
    ],
    visibility = ["//visibility:public"],
)
