load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "health_module",
    hdrs = ["base_health_module.h"],
    deps = [
        "//code/client/messages/proto:damage_cc_proto",
        "//code/client/messages/proto:heal_cc_proto",
        "//code/client/modules/base:base_module",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "damage_type_resist_health_module",
    srcs = ["damage_type_resist_health_module.cpp"],
    hdrs = ["damage_type_resist_health_module.h"],
    deps = [
        "@yaml-cpp",
        ":health_module",
        "//code/client/modules/descriptors:damage_type_amount_descriptor",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "damage_type_resist_percent_health_module",
    srcs = ["damage_type_resist_percent_health_module.cpp"],
    hdrs = ["damage_type_resist_percent_health_module.h"],
    deps = [
        "@yaml-cpp",
        ":health_module",
        "//code/client/modules/descriptors:damage_type_percent_descriptor",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "heal_type_increase_health_module",
    srcs = ["heal_type_increase_health_module.cpp"],
    hdrs = ["heal_type_increase_health_module.h"],
    deps = [
        "@yaml-cpp",
        ":health_module",
        "//code/client/modules/descriptors:heal_type_amount_descriptor",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "status_effect_immune_health_module",
    srcs = ["status_effect_immune_health_module.cpp"],
    hdrs = ["status_effect_immune_health_module.h"],
    deps = [
        "@yaml-cpp",
        ":health_module",
        "//code/client/modules/descriptors:status_effect_descriptor",
        "//code/client/status_effect:status_effect_library",
    ],
    visibility = ["//visibility:public"],
)
