load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "damage_module",
    hdrs = ["base_damage_module.h"],
    deps = [
        "@yaml-cpp",
        "//code/client/messages/proto:damage_cc_proto",
        "//code/client/modules/base:base_module",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "damage_type_apply_damage_module",
    srcs = ["damage_type_apply_damage_module.cpp"],
    hdrs = ["damage_type_apply_damage_module.h"],
    deps = [
        "@yaml-cpp",
        ":damage_module",
        "//code/client/modules/descriptors:damage_type_amount_descriptor",
        "//code/core:log_manager",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "status_effect_apply_damage_module",
    srcs = ["status_effect_apply_damage_module.cpp"],
    hdrs = ["status_effect_apply_damage_module.h"],
    deps = [
        "@yaml-cpp",
        ":damage_module",
        "//code/client/modules/descriptors:status_effect_descriptor",
        "//code/core:log_manager",
    ],
    visibility = ["//visibility:public"],
)
