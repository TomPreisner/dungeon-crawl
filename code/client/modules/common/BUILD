load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "base_module",
    hdrs = ["base_module.h"],
    deps = ["@yaml-cpp"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "damage_type_module_common",
    srcs = ["damage_type_module_common.cpp"],
    hdrs = ["damage_type_module_common.h"],
    deps = [
        "@yaml-cpp",
        ":base_module",
        "//code/client/damage/proto:damage_cc_proto",
        "//code/core:log_manager",
    ],
    visibility = ["//visibility:public"],
)
