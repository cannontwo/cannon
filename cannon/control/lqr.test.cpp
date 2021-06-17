#include <catch2/catch.hpp>

#include <cannon/control/lqr.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::control;
using namespace cannon::log;

TEST_CASE("LQR", "[control]") {
  // Double integrator
  MatrixXd A(2, 2);
  A << 0, 1,
       0, 0;

  MatrixXd B(2, 1);
  B << 0,
       1;

  MatrixXd Q(2, 2);
  Q << 1, 0,
       0, 1;

  MatrixXd R(1, 1);
  R << 1;

  LLT<MatrixXd> R_cholesky(R);
  if (R_cholesky.info() != Eigen::Success)
    throw std::runtime_error("Control cost matrix was not positive definite");

  MatrixXd K = continuous_algebraic_riccati_equation(A, B, Q, R_cholesky);
  log_info("Found LQR gain", K);

  RowVectorXd K_matlab(2);
  K_matlab << 1.0, 1.73205080756888;
  REQUIRE(K_matlab.isApprox(K));
}
