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
    name = "effect_clear",
    srcs = ["effect_clear.cpp"],
    hdrs = ["effect_clear.h"],
    deps = [
        "@yaml-cpp",
        ":effect_base",
        ":effect_data",
        ":effect_type",
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
        ":effect_base",
        ":effect_data",
        ":effect_type",
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
        ":effect_damage",
        ":effect_data",
        ":effect_type",
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
        ":effect_base",
        ":effect_data",
        ":effect_type",
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
        ":effect_heal",
        ":effect_data",
        ":effect_type",
        "//code/client/messages/proto:heal_cc_proto",
        "//code/core:log_manager",
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
        ":effect_clear",
        ":effect_data",
        ":effect_damage",
        ":effect_damage_multiply",
        ":effect_heal",
        ":effect_heal_multiply",
        ":effect_type",
    ],
    visibility = ["//visibility:public"],
)
