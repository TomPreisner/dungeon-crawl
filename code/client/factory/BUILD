load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "character_factory",
    srcs = ["character_factory.cpp"],
    hdrs = ["character_factory.h"],
    deps = [
        "//code/client/character/enemy:all_enemy",
        "//code/client/character/npc:all_npc",
        "//code/client/character/player:all_player",
        "//code/client/character/player_ally:all_player_ally"
    ],
    visibility = ["//visibility:public"],
)
