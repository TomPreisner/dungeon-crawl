load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "all_player",
    deps = [
        ":player_base"
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "player_base",
    srcs = ["player_base.cpp"],
    hdrs = ["player_base.h"],
    deps = [
        "//code/client/character/base:character_base"
    ],
)
