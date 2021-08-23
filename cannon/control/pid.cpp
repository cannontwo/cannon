#include <cannon/control/pid.hpp>

using namespace cannon::control;

PidController::PidController(unsigned int state_dim, unsigned int control_dim)
    : state_dim_(state_dim), control_dim_(control_dim),
      ref_(VectorXd::Zero(state_dim_)),
      last_error_state_(VectorXd::Zero(state_dim_)),
      integrated_error_(VectorXd::Zero(state_dim_)),
      Kp_(MatrixXd::Zero(control_dim_, state_dim_)),
      Ki_(MatrixXd::Zero(control_dim_, state_dim_)),
      Kd_(MatrixXd::Zero(control_dim_, state_dim_)) {}

VectorXd PidController::get_control(const Ref<const VectorXd>& state) {
  auto error_state = ref_ - state;
  auto p_part = Kp_ * error_state;

  auto i_part = Ki_ * integrated_error_;

  auto error_diff = error_state - last_error_state_;
  auto d_part = Kd_ * error_diff;

  // Update tracked components
  last_error_state_ = error_state;
  integrated_error_ += error_state;

  return p_part + i_part + d_part;
}
