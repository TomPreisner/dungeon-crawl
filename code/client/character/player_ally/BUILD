load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "all_player_ally",
    deps = [
        ":player_ally_base"
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "player_ally_base",
    srcs = ["player_ally_base.cpp"],
    hdrs = ["player_ally_base.h"],
    deps = [
        "//code/client/character/base:character_base"
    ],
)
