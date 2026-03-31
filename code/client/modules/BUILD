load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "module_factory",
    srcs = ["module_factory.cpp"],
    hdrs = [
        "module_factory.h",
        "module_registry.inl",
    ],
    deps = [
        "@yaml-cpp",
        "//code/client/modules/common:base_module",
        "//code/client/modules/damage:damage_type_apply_module",
        "//code/client/modules/health:damage_type_resist_module",
        "//code/client/modules/health:damage_type_resist_percent_module",
        "//code/core:log_manager",
    ],
    visibility = ["//visibility:public"],
)
