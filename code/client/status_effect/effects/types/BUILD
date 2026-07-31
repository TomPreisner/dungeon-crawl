load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "effect_clear",
    srcs = ["effect_clear.cpp"],
    hdrs = ["effect_clear.h"],
    deps = [
        "@yaml-cpp",
        "//code/client/status_effect/effects:effect_base",
        "//code/client/status_effect/effects:effect_data",
        "//code/client/status_effect/effects:effect_type",
        "//code/core:log_manager",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "effect_damage",
    srcs = ["effect_damage.cpp"],
    hdrs = ["effect_damage.h"],
    deps = [
        "@yaml-cpp",
        "//code/client/status_effect/effects:effect_base",
        "//code/client/status_effect/effects:effect_data",
        "//code/client/status_effect/effects:effect_type",
        "//code/client/messages/proto:damage_cc_proto",
        "//code/core:log_manager",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "effect_damage_multiply",
    srcs = ["effect_damage_multiply.cpp"],
    hdrs = ["effect_damage_multiply.h"],
    deps = [
        "@yaml-cpp",
        "//code/client/status_effect/effects/types:effect_damage",
        "//code/client/status_effect/effects:effect_data",
        "//code/client/status_effect/effects:effect_type",
        "//code/client/messages/proto:damage_cc_proto",
        "//code/core:log_manager",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "effect_heal",
    srcs = ["effect_heal.cpp"],
    hdrs = ["effect_heal.h"],
    deps = [
        "@yaml-cpp",
        "//code/client/status_effect/effects:effect_base",
        "//code/client/status_effect/effects:effect_data",
        "//code/client/status_effect/effects:effect_type",
        "//code/client/messages/proto:heal_cc_proto",
        "//code/core:log_manager",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "effect_heal_multiply",
    srcs = ["effect_heal_multiply.cpp"],
    hdrs = ["effect_heal_multiply.h"],
    deps = [
        "@yaml-cpp",
        "//code/client/status_effect/effects/types:effect_heal",
        "//code/client/status_effect/effects:effect_data",
        "//code/client/status_effect/effects:effect_type",
        "//code/client/messages/proto:heal_cc_proto",
        "//code/core:log_manager",
    ],
    visibility = ["//visibility:public"],
)
