load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "effect_data",
    hdrs = ["effect_data.h"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "effect_base",
    srcs = ["base_effect.cpp"],
    hdrs = ["base_effect.h"],
    deps = [
        "@yaml-cpp",
        "//code/core:log_manager",
        ":effect_data",
        ":effect_type",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "effect_type",
    srcs = ["effect_type.cpp"],
    hdrs = ["effect_type.h"],
    deps = [
        "@yaml-cpp",
        "//code/core:log_manager",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "effect_factory",
    srcs = ["effect_factory.cpp"],
    hdrs = ["effect_factory.h"],
    deps = [
        "@yaml-cpp",
        "//code/core:log_manager",
        ":effect_base",
        ":effect_data",
        ":effect_type",
        "//code/client/status_effect/effects/types:effect_clear",
        "//code/client/status_effect/effects/types:effect_damage",
        "//code/client/status_effect/effects/types:effect_damage_multiply",
        "//code/client/status_effect/effects/types:effect_heal",
        "//code/client/status_effect/effects/types:effect_heal_multiply",
    ],
    visibility = ["//visibility:public"],
)
