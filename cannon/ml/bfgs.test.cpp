#include <catch2/catch.hpp>

#include <cannon/ml/bfgs.hpp>
#include <cannon/math/func/rosenbrock.hpp>

using namespace cannon::ml;
using namespace cannon::math;

TEST_CASE("BFGS", "[ml]") {
  BFGSOptimizer opt(rosenbrock, rosenbrock_grad);

  Vector2d x = Vector2d::Zero();
  log_info("rosenbrock(start) = ", rosenbrock(x));

  OptimizationResult result = opt.optimize(x);
  log_info(result);

  REQUIRE(result.objective < 1e-10);
}
