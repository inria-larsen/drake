# -*- python -*-
# This file contains rules for Bazel; see drake/doc/bazel.rst.

load(
    "//tools:drake.bzl",
    "drake_cc_binary",
    "drake_cc_googletest",
    "drake_cc_library",
)
load("//tools:lint.bzl", "add_lint_tests")

package(default_visibility = ["//visibility:public"])

drake_cc_library(
    name = "dragway",
    srcs = [
        "branch_point.cc",
        "junction.cc",
        "lane.cc",
        "road_geometry.cc",
        "segment.cc",
    ],
    hdrs = [
        "branch_point.h",
        "junction.h",
        "lane.h",
        "road_geometry.h",
        "segment.h",
    ],
    deps = [
        "//drake/automotive/maliput/api",
        "//drake/common:essential",
        "//drake/common:unused",
        "//drake/math:saturate",
    ],
)

drake_cc_binary(
    name = "dragway_to_urdf",
    srcs = ["dragway_to_urdf.cc"],
    deps = [
        ":dragway",
        "//drake/automotive/maliput/utility",
        "//drake/common:essential",
        "//drake/common:text_logging_gflags",
        "//drake/thirdParty:spruce",
        "@com_github_gflags_gflags//:gflags",
    ],
)

# === test/ ===

drake_cc_googletest(
    name = "dragway_test",
    # Test size increased to not timeout when run with Valgrind.
    size = "medium",
    srcs = ["test/dragway_test.cc"],
    deps = [
        ":dragway",
        "//drake/automotive/maliput/api/test_utilities",
    ],
)

add_lint_tests()
