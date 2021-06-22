#include <cannon/control/lqr.hpp>

#include <cannon/log/registry.hpp>

using namespace cannon::control;
using namespace cannon::log;

LQRController::LQRController(const Ref<const VectorXd> &q0,
                             unsigned int control_dim, LinearizationFunction f,
                             bool continuous)
    : q0_(q0), Q_(MatrixXd::Identity(q0.size(), q0.size())),
      R_(MatrixXd::Identity(control_dim, control_dim)), R_cholesky_(R_),
      linearization_(f), continuous_(continuous) {
  assert(R_cholesky_.info() == Eigen::Success);

  compute_lqr_gain_();
}

LQRController::LQRController(const Ref<const VectorXd> &q0,
                             const Ref<const MatrixXd> &Q,
                             const Ref<const MatrixXd> &R,
                             LinearizationFunction f, bool continuous)
    : q0_(q0), Q_(Q), R_(R), R_cholesky_(R), linearization_(f),
      continuous_(continuous) {
  assert(q0_.size() == Q_.rows());
  assert(Q_.cols() == Q_.rows());
  assert(R_cholesky_.info() == Eigen::Success);

  compute_lqr_gain_();
}

VectorXd LQRController::compute_control(const Ref<const VectorXd>& q) {
  return -K_ * (q - q0_);
}

MatrixXd LQRController::get_linear_gain() {
  return -K_;
}

VectorXd LQRController::get_control_offset() {
  return K_ * q0_;
}


void LQRController::compute_lqr_gain_() {
  MatrixXd A(q0_.size(), q0_.size());
  MatrixXd B(q0_.size(), R_cholesky_.rows());

  linearization_(q0_, A, B);

  // Handle offset vector c
  if (continuous_) {
    K_ = continuous_algebraic_riccati_equation(A, B, Q_, R_cholesky_);
  } else {
    throw std::runtime_error("Discrete-time Algebraic Riccati Equation not implemented yet");
  }
}

void LQRController::set_target(const Ref<const VectorXd>& q) {
  assert(q.size() == q0_.size());
  q0_ = q;

  compute_lqr_gain_();
}

// Free Functions

MatrixXd cannon::control::continuous_algebraic_riccati_equation(
    const Ref<const MatrixXd> &A, const Ref<const MatrixXd> &B,
    const Ref<const MatrixXd> &Q, const LLT<MatrixXd> &R_cholesky) {
  // Code taken from the Drake project, covered by the 3-clause BSD license. See:
  // https://github.com/RobotLocomotion/drake/blob/master/math/continuous_algebraic_riccati_equation.cc
  // https://github.com/RobotLocomotion/drake/blob/master/LICENSE.TXT
  
  // R. Byers. Solving the algebraic Riccati equation with the matrix sign
  // function. Linear Algebra Appl., 85:267–279, 1987
  // Added determinant scaling to improve convergence (converges in rough half
  // the iterations with this)
  
  const Index n = B.rows();
  MatrixXd H(2*n, 2*n);

  H << A,   B * R_cholesky.solve(B.transpose()),
       Q, -A.transpose();

  MatrixXd Z = H;
  MatrixXd Z_old;

  // Configurable options
  const double tolerance = 1e-9;
  const double max_iterations = 100;

  double relative_norm;
  size_t iteration = 0;
  const double p = static_cast<double>(Z.rows());

  do {
    Z_old = Z;

    double ck = std::pow(std::abs(Z.determinant()), -1.0 / p);
    Z *= ck;
    Z = Z - 0.5 * (Z - Z.inverse());
    relative_norm = (Z - Z_old).norm();

    iteration++;
  } while (iteration < max_iterations && relative_norm > tolerance);

  MatrixXd W11 = Z.block(0, 0, n, n);
  MatrixXd W12 = Z.block(0, n, n, n);
  MatrixXd W21 = Z.block(n, 0, n, n);
  MatrixXd W22 = Z.block(n, n, n, n);

  MatrixXd lhs(2*n, n);
  MatrixXd rhs(2*n, n);
  MatrixXd eye = MatrixXd::Identity(n, n);
  lhs << W12, 
         W22 + eye;
  rhs << W11 + eye,
         W21;

  JacobiSVD<MatrixXd> svd(lhs, Eigen::ComputeThinU | Eigen::ComputeThinV);

  return R_cholesky.solve(B.transpose() * svd.solve(rhs));
}
