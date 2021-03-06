#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "drake/common/drake_copyable.h"
#include "drake/lcmt_plan_eval_debug_info.hpp"
#include "drake/systems/controllers/plan_eval/plan_eval_base_system.h"

namespace drake {
namespace examples {
namespace qp_inverse_dynamics {

/**
 * This class extends PlanEvalBaseSystem. It outputs a QpInput that contains
 * a single generalized acceleration objective to track the nominal
 * instantaneous generalized position, velocity and acceleration.
 * Assuming no external contacts, the control law is:
 * <pre>
 *   vdot_output = Kp(q* - q) + Kd(v* - v) + vdot*,
 * </pre>
 * where Kp and Kd are the position and velocity gains, and q*, v*, and vdot* are
 * the nominal generalized position, velocity and acceleration.
 * This system assumes that |q| = |v|, and model being controlled contains no
 * floating base joints.
 */
class ManipulatorMoveJointPlanEvalSystem
    : public systems::controllers::plan_eval::PlanEvalBaseSystem {
 public:
  DRAKE_NO_COPY_NO_MOVE_NO_ASSIGN(ManipulatorMoveJointPlanEvalSystem)

  /**
   * Constructor. Aborts if @p robot has unequal number of generalized positions
   * and velocities or contains any floating base joint.
   * @param robot Pointer to a RigidBodyTree, whose life span must be longer
   * than this instance.
   * @param alias_groups_file_name Path to the alias groups file that describes
   * the robot's topology for the controller.
   * @param param_file_name Path to the config file for the controller.
   * @param dt Time step
   */
  ManipulatorMoveJointPlanEvalSystem(const RigidBodyTree<double>* robot,
                                     const std::string& alias_groups_file_name,
                                     const std::string& param_file_name,
                                     double dt);

  /**
   * Initializes the plan and gains. Must be called before execution.
   */
  void Initialize(systems::State<double>* state);

  /**
   * Initializes the plan and gains. Must be called before execution.
   */
  void Initialize(systems::Context<double>* context) {
    systems::State<double>* servo_state = context->get_mutable_state();
    Initialize(servo_state);
  }

  /**
   * Returns the input port for desired position and velocity.
   */
  inline const systems::InputPortDescriptor<double>&
  get_input_port_desired_state() const {
    return get_input_port(input_port_index_desired_state_);
  }

  /**
   * Returns the input port for desired acceleration.
   */
  inline const systems::InputPortDescriptor<double>&
  get_input_port_desired_acceleration() const {
    return get_input_port(input_port_index_desired_acceleration_);
  }

  /**
   * Returns the output port for debugging information.
   */
  inline const systems::OutputPort<double>& get_output_port_debug_info() const {
    return get_output_port(output_port_index_debug_info_);
  }

 private:
  int get_num_extended_abstract_states() const override { return 2; }

  void DoExtendedCalcUnrestrictedUpdate(
      const systems::Context<double>& context,
      systems::State<double>* state) const override;

  // This is the calculator method for the output port.
  void OutputDebugInfo(const systems::Context<double>& context,
                       lcmt_plan_eval_debug_info* output) const;

  int input_port_index_desired_state_{};
  int input_port_index_desired_acceleration_{};
  int output_port_index_debug_info_{};

  int abs_state_index_plan_{};
  int abs_state_index_debug_{};
};

}  // namespace qp_inverse_dynamics
}  // namespace examples
}  // namespace drake
