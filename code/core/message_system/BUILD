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
    ],
    # visibility limited on purpose
    visibility = [
        "//code/core/message_system/test:__pkg__",
        "//code/core/message_system/test/example:__pkg__",
    ],
)

cc_library(
    name = "message_system",
    srcs = ["message_pub_sub_base.h"], # visibility hidden to limit inheritence
    hdrs = [
        "message_publisher.h",
        "message_subscriber.h",
        "message_switchboard.h"
    ],
    deps = [
        ":message_dispatch",
    ],
    visibility = ["//visibility:public"],
)
