# -*- python -*-
# This file contains rules for Bazel; see drake/doc/bazel.rst.

load("//tools:drake.bzl", "drake_cc_googletest", "drake_cc_library")
load("//tools:lint.bzl", "add_lint_tests")

package(default_visibility = ["//visibility:public"])

drake_cc_library(
    name = "bead_on_a_wire",
    srcs = ["bead_on_a_wire.cc"],
    hdrs = [
        "bead_on_a_wire.h",
        "bead_on_a_wire-inl.h",
    ],
    deps = [
        "//drake/common:essential",
        "//drake/systems/framework:leaf_system",
    ],
)

# === test/ ===

drake_cc_googletest(
    name = "bead_on_a_wire_test",
    deps = [
        ":bead_on_a_wire",
        "//drake/systems/analysis",
    ],
)

add_lint_tests()
