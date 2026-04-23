load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "message_example",
    srcs = ["actor_classes.cpp"],
    hdrs = [
        "actor_classes.h",
        "message_types.h",
        "simulation.h"
    ],
    deps = [
        "//code/core/message_system:message_dispatch",
        "//code/core/message_system:message_system",
    ],
    visibility = ["//visibility:public"],
)
