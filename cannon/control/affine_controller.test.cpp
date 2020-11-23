#include <catch2/catch.hpp>

#include <cannon/control/affine_controller.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::control;
using namespace cannon::log;

TEST_CASE("Gradient applied to affine controller", "[control]") {
  AffineController c(2, 1, 1e-4);

  VectorXd s(2);
  s << 1.0,
       0.0;

  log_info("Got control", c.get_action(s));
  c.apply_gradient(VectorXd::Zero(1), MatrixXd::Zero(1, 2));
  log_info("Applied fake gradient");
}
