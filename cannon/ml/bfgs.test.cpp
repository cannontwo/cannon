#include <catch2/catch.hpp>

#include <cannon/ml/bfgs.hpp>
#include <cannon/math/func/rosenbrock.hpp>
#include <cannon/ml/optimization_result.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::ml;
using namespace cannon::math;
using namespace cannon::log;

TEST_CASE("BFGS", "[ml]") {
  BFGSOptimizer opt(rosenbrock, rosenbrock_grad);

  Vector2d x = Vector2d::Zero();
  log_info("rosenbrock(start) = ", rosenbrock(x));

  OptimizationResult result = opt.optimize(x);
  log_info(result);

  REQUIRE(result.objective < 1e-10);
}
