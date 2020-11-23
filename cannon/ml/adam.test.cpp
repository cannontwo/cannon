#include <catch2/catch.hpp>

#include <cannon/ml/adam.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::ml;
using namespace cannon::log;

TEST_CASE("Adam", "[ml]") {
  AdamOptimizer a(2, 1, 1e-1); 
  VectorXd state = VectorXd::Random(2);

  for (int i = 0; i < 100; i++) {
    VectorXd grad = -2.0 * state;
    state = a.apply_update(state, grad);
    log_info("On iteration", i, "state is", state);
  }

  REQUIRE(state.norm() < 0.01);
}
