load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "damage_type_amount_descriptor",
    srcs = ["damage_type_amount_descriptor.cpp"],
    hdrs = ["damage_type_amount_descriptor.h"],
    deps = [
        "@yaml-cpp",
        "//code/client/modules/descriptors/base:damage_type_descriptor_base",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "damage_type_percent_descriptor",
    srcs = ["damage_type_percent_descriptor.cpp"],
    hdrs = ["damage_type_percent_descriptor.h"],
    deps = [
        ":damage_type_amount_descriptor",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "heal_type_amount_descriptor",
    srcs = ["heal_type_amount_descriptor.cpp"],
    hdrs = ["heal_type_amount_descriptor.h"],
    deps = [
        "@yaml-cpp",
        "//code/client/modules/descriptors/base:heal_type_descriptor_base",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "status_effect_descriptor",
    srcs = ["status_effect_descriptor.cpp"],
    hdrs = ["status_effect_descriptor.h"],
    deps = [
        "@yaml-cpp",
        "//code/client/modules/descriptors/base:status_effect_descriptor_base",
        "//code/client/status_effect:status_effect_library",
    ],
    visibility = ["//visibility:public"],
)
