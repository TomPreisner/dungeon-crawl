load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "all_npc",
    deps = [
        ":npc_base"
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "npc_base",
    srcs = ["npc_base.cpp"],
    hdrs = ["npc_base.h"],
    deps = [
        "//code/client/character/base:character_base"
    ],
)
