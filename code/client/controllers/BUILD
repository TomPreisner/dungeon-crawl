load("@rules_cc//cc:cc_binary.bzl", "cc_binary")
load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "level_director",
    srcs = ["level_director.cpp"],
    hdrs = ["level_director.h"],
    deps = [
        ":stage_manager",
        ":battle_coordinator",
    ],
)

cc_library(
    name = "battle_coordinator",
    srcs = ["battle_coordinator.cpp"],
    hdrs = ["battle_coordinator.h"],
)

cc_library(
    name = "stage_manager",
    srcs = ["stage_manager.cpp"],    
    hdrs = ["stage_manager.h"],
    deps = [
        ":casting_director",
        ":prop_manager",
    ],
)

cc_library(
    name = "prop_manager",
    srcs = ["prop_manager.cpp"],
    hdrs = ["prop_manager.h"],
)

cc_library(
    name = "casting_director",
    srcs = ["casting_director.cpp"],
    hdrs = ["casting_director.h"],
)