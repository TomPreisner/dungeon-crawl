load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "health_module",
    hdrs = ["health_module.h"],
    deps = [
        "//code/client/damage/proto:damage_cc_proto",   
        "//code/client/health/proto:heal_cc_proto",        
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "damage_type_resist_module",
    srcs = ["damage_type_resist_module.cpp"],
    hdrs = ["damage_type_resist_module.h"],
    deps = [
        "@yaml-cpp",
        ":health_module",
        "//code/client/modules/common:damage_type_module_common",  
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "damage_type_resist_percent_module",
    srcs = ["damage_type_resist_percent_module.cpp"],
    hdrs = ["damage_type_resist_percent_module.h"],
    deps = [
        ":damage_type_resist_module",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "heal_type_increase_module",
    srcs = ["heal_type_increase_module.cpp"],
    hdrs = ["heal_type_increase_module.h"],
    deps = [
        "@yaml-cpp",
        ":health_module",
        "//code/client/modules/common:heal_type_module_common",  
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "status_effect_immune_module",
    srcs = ["status_effect_immune_module.cpp"],
    hdrs = ["status_effect_immune_module.h"],
    deps = [
        "@yaml-cpp",
        ":health_module",
        "//code/client/modules/common:status_effect_module_common",
        "//code/client/status_effect:status_effect_library",
    ],
    visibility = ["//visibility:public"],
)
