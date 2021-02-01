#include <catch2/catch.hpp>

#include <cstdlib>

#include <cannon/ml/linear_programming.hpp>
#include <cannon/log/registry.hpp>
#include <cannon/math/nearly_equal.hpp>

using namespace cannon::ml;
using namespace cannon::log;
using namespace cannon::math;

TEST_CASE("LinearProgramming", "[ml]") {
  // Example from
  // https://personal.utdallas.edu/~scniu/OPRE-6201/documents/LP2-LP_Examples.html
  try {
    VectorXd objective_vec(2);
    objective_vec << -5.0,
                     -7.0;

    VectorXd lower = VectorXd::Zero(2);
    VectorXd upper(2);
    upper << 1000,
             1000;

    LPOptimizer opt(lower, upper, objective_vec);

    VectorXd constraint_vec(2);
    constraint_vec << 650,
                      500;

    MatrixXd constraint_mat(2, 2);
    constraint_mat << 3, 4,
                      2, 3;

    opt.add_constraint(constraint_mat, constraint_vec);
  
    auto result = opt.optimize();
    log_info("Got result", result.solution);
    log_info("with objective value", result.objective);

    REQUIRE(nearly_equal(result.objective, -1137.5));
  } catch (GRBException e) {
    log_error("Error code =", e.getErrorCode());
    log_error(e.getMessage());
  } catch (...) {
    log_error("Exception during optimization.");
  }
}
