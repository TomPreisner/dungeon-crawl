load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "base_status_effect",
    hdrs = ["base_status_effect.h"],
    deps = [
        "@yaml-cpp",
        ":effect",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "effect",
    srcs = ["effect.cpp"],
    hdrs = ["effect.h"],
    deps = ["@yaml-cpp"],
    visibility = ["//visibility:public"],
)
