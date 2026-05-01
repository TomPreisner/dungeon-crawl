load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "damage_module",
    hdrs = ["damage_module.h"],
    deps = [
        "@yaml-cpp",
        "//code/client/messages/proto:damage_cc_proto",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "damage_type_apply_module",
    srcs = ["damage_type_apply_module.cpp"],
    hdrs = ["damage_type_apply_module.h"],
    deps = [
        "@yaml-cpp",
        ":damage_module",
        "//code/client/modules/common:damage_type_module_common",
        "//code/core:log_manager",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "status_effect_apply_module",
    srcs = ["status_effect_apply_module.cpp"],
    hdrs = ["status_effect_apply_module.h"],
    deps = [
        "@yaml-cpp",
        ":damage_module",
        "//code/client/modules/common:status_effect_module_common",
        "//code/core:log_manager",
    ],
    visibility = ["//visibility:public"],
)
