#include <cannon/control/pid.hpp>

#include <cannon/log/registry.hpp>

using namespace cannon::log;
using namespace cannon::control;

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
