#include <cassert>
#include <utility>

#include <cannon/log/registry.hpp>
#include <cannon/control/affine_controller.hpp>

using namespace cannon::control;
using namespace cannon::log;

VectorXd AffineController::get_action(const VectorXd& state) const {
  assert(state.size() == state_dim_);

  return (K_ * state) + k_;
}

std::pair<VectorXd, MatrixXd> AffineController::get_mats() const {
  return std::make_pair(k_, K_);
}

void AffineController::apply_gradient(const VectorXd& k_gradient, const MatrixXd& K_gradient) {
  assert(k_gradient.size() == action_dim_);
  assert(K_gradient.rows() == action_dim_);
  assert(K_gradient.cols() == state_dim_);

  if (use_adam_) {
    K_ = K_opt_.apply_update(K_, K_gradient).eval();
    k_ = k_opt_.apply_update(k_, k_gradient).eval();
  } else {
    // TODO Encapsulate this behavior by making AffineController take a templated optimizer
    K_ += learning_rate_ * K_gradient;
    k_ += learning_rate_ * k_gradient;
  }

  learning_rate_ = initial_learning_rate_ * (1.0 / (1.0 + (decay_ * overall_t_)));
  K_opt_.set_learning_rate(learning_rate_);
  k_opt_.set_learning_rate(learning_rate_);
  overall_t_ += 1;
}

void AffineController::set_k(const VectorXd& k) {
  assert(k.size() == action_dim_);
  k_ = k;
}

void AffineController::set_K(const MatrixXd& K) {
  assert(K.rows() == action_dim_);
  assert(K.cols() == state_dim_);
  K_ = K;
}
