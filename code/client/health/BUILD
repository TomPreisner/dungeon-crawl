load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "health_manager",
    srcs = ["health_manager.cpp"],
    hdrs = ["health_manager.h"],
    deps = [
        "@boost.uuid",
        "@yaml-cpp",
        "//code/client/damage/proto:damage_cc_proto",
        "//code/client/health/proto:heal_cc_proto",
        "//code/client/modules/health:health_module",
        "//code/core:log_manager",
    ],
    visibility = ["//visibility:public"],
)
