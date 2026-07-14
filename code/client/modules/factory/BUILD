load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "health_module_factory",
    srcs = ["health_module_factory.cpp"],
    hdrs = [
        "health_module_factory.h",
        "health_module_registry.inl",
    ],
    deps = [
        "@yaml-cpp",
        "//code/client/modules/descriptors/base:base_descriptor",
        "//code/client/modules/health:health_module",
        "//code/client/modules/health:damage_type_resist_health_module",
        "//code/client/modules/health:damage_type_resist_percent_health_module",
        "//code/client/modules/health:heal_type_increase_health_module",
        "//code/client/modules/health:status_effect_immune_health_module",
        "//code/core:log_manager",
    ],
    visibility = ["//visibility:public"],
)
