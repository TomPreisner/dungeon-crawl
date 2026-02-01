workspace(name = "dungeon_crawler")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "rules_license",
    urls = [
        "https://mirror.bazel.build/github.com/bazelbuild/rules_license/releases/download/1.0.0/rules_license-1.0.0.tar.gz",
        "https://github.com/bazelbuild/rules_license/releases/download/1.0.0/rules_license-1.0.0.tar.gz",
    ],
    sha256 = "26d4021f6898e23b82ef953078389dd49ac2b5618ac564ade4ef87cced147b38",
)

http_archive(
    name = "rules_cc",
    sha256 = "458b658277ba51b4730ea7a2020efdf1c6dcadf7d30de72e37f4308277fa8c01",
    strip_prefix = "rules_cc-0.2.16",
    url = "https://github.com/bazelbuild/rules_cc/releases/download/0.2.16/rules_cc-0.2.16.tar.gz",
)

load("@rules_cc//cc:extensions.bzl", "compatibility_proxy_repo")

compatibility_proxy_repo()