load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "all_enemy",
    deps = [
        ":enemy_base"
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "enemy_base",
    srcs = ["enemy_base.cpp"],
    hdrs = ["enemy_base.h"],
    deps = [
        "//code/client/character/base:character_base"
    ],
)
