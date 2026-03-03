load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "character_base",
    srcs = ["character_base.cpp"],
    hdrs = ["character_base.h"],
    deps = [
        "@behaviortree_cpp//:bt_cpp",
        "//code/client/object:object_dynamic"
    ],
    visibility = ["//visibility:public"],
)
