# -*- python -*-
# This file contains rules for Bazel; see drake/doc/bazel.rst.

load("//tools:drake.bzl", "drake_cc_googletest", "drake_cc_library")
load("//tools:lint.bzl", "add_lint_tests")

package(default_visibility = ["//visibility:public"])

filegroup(
    name = "models",
    testonly = 1,
    srcs = glob([
        "**/*.obj",
        "**/*.sdf",
        "**/*.urdf",
        "**/*.xml",
    ]),
)

# === test/ ===

drake_cc_googletest(
    name = "cylinder_torque_free_analytical_solution",
    size = "medium",
    data = [":models"],
    deps = [
        "//drake/common:find_resource",
        "//drake/common/test_utilities:eigen_matrix_compare",
        "//drake/math:autodiff",
        "//drake/multibody/parsers",
        "//drake/multibody/rigid_body_plant",
        "//drake/systems/analysis:runge_kutta3_integrator",
        "//drake/systems/framework:diagram",
        "//drake/systems/framework:diagram_builder",
        "//drake/systems/primitives:constant_vector_source",
    ],
)

add_lint_tests()
