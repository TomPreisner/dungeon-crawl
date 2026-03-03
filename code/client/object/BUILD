load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "object_base",
    srcs = ["object_base.cpp"],
    hdrs = ["object_base.h"],
    deps = ["@eigen"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "object_static",
    srcs = ["object_static.cpp"],
    hdrs = ["object_static.h"],
    deps = [":object_base"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "object_dynamic",
    srcs = ["object_dynamic.cpp"],
    hdrs = ["object_dynamic.h"],
    deps = [":object_base"],
    visibility = ["//visibility:public"],
)
