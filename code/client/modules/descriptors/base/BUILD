load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "base_descriptor",
    hdrs = ["base_descriptor.h"],
    deps = ["@yaml-cpp"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "damage_type_descriptor_base",
    srcs = ["damage_type_descriptor_base.cpp"],
    hdrs = ["damage_type_descriptor_base.h"],
    deps = [
        "@yaml-cpp",
        ":base_descriptor",
        "//code/client/messages/proto:damage_cc_proto",
        "//code/core:log_manager",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "heal_type_descriptor_base",
    srcs = ["heal_type_descriptor_base.cpp"],
    hdrs = ["heal_type_descriptor_base.h"],
    deps = [
        "@yaml-cpp",
        ":base_descriptor",
        "//code/client/messages/proto:heal_cc_proto",
        "//code/core:log_manager",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "status_effect_descriptor_base",
    srcs = ["status_effect_descriptor_base.cpp"],
    hdrs = ["status_effect_descriptor_base.h"],
    deps = [
        "@yaml-cpp",
        ":base_descriptor",
        "//code/client/status_effect:status_effect",
        "//code/client/status_effect:status_effect_library",
        "//code/core:log_manager",
    ],
    visibility = ["//visibility:public"],
)
