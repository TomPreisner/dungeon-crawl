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
