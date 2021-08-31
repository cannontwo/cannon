#include <cannon/control/pid.hpp>

#include <cannon/log/registry.hpp>
#include <cannon/geom/trajectory.hpp>
#include <cannon/math/interp.hpp>

using namespace cannon::log;
using namespace cannon::control;
using namespace cannon::geom;
using namespace cannon::math;

PidController::PidController(unsigned int state_dim, unsigned int control_dim,
                             double timestep)
    : state_dim_(state_dim), control_dim_(control_dim), time_step_(timestep),
      ref_(VectorXd::Zero(state_dim_)),
      penultimate_error_state_(VectorXd::Zero(state_dim_)),
      last_error_state_(VectorXd::Zero(state_dim_)),
      last_control_(VectorXd::Zero(control_dim_)),
      Kp_(MatrixXd::Zero(control_dim_, state_dim_)),
      Ki_(MatrixXd::Zero(control_dim_, state_dim_)),
      Kd_(MatrixXd::Zero(control_dim_, state_dim_)) {}

VectorXd PidController::get_control(const Ref<const VectorXd>& state) {
  auto error_state = ref_ - state;

  // For derivations of these computations, see
  // http://portal.ku.edu.tr/~cbasdogan/Courses/Robotics/projects/Discrete_PID.pdf,
  // http://www.irphouse.com/ijec/ijecv2n1_05.pdf
  VectorXd p_part = Kp_ * (error_state - last_error_state_);

  VectorXd i_part = Ki_ * time_step_ * error_state;

  VectorXd d_part =
      (Kd_ / time_step_) *
      (error_state - 2 * last_error_state_ + penultimate_error_state_);

  penultimate_error_state_ = last_error_state_;
  last_error_state_ = error_state;

  VectorXd control = last_control_ + p_part + i_part + d_part;
  last_control_ = control;

  return control;
}

void PidController::reset() {
  penultimate_error_state_ = VectorXd::Zero(state_dim_);
  last_error_state_ = VectorXd::Zero(state_dim_);
  last_control_ = VectorXd::Zero(control_dim_);
}

ControlledTrajectory cannon::control::get_pid_controlled_trajectory(
    std::function<VectorXd(const Ref<const VectorXd> &,
                           const Ref<const VectorXd> &)>
        system,
    const Trajectory &traj, PidController &controller,
    unsigned int controlled_dims, unsigned int total_dims, double timestep) {

  controller.reset();

  // Get smooth interpolated trajectory
  auto plan = traj.interp();

  double time = 0.0;
  VectorXd state(total_dims);
  state.head(controlled_dims) = traj(time);
  ControlledTrajectory executed;

  while (time < traj.length()) {
    // Set tracking ref
    controller.ref() = plan(time).head(controlled_dims);
    auto pid_control = controller.get_control(state.head(controlled_dims));
    executed.push_back(state, pid_control, time);

    // Step system
    state = system(state, pid_control);
    time += timestep;
  }

  return executed;
}
