#include <catch2/catch.hpp>

#include <cannon/math/multivariate_normal.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::math;
using namespace cannon::log;

TEST_CASE("MultivariateNormal", "[math]") {
  MatrixXd covar = MatrixXd::Identity(2, 2);
  covar(0, 0) = 10.0;
  MultivariateNormal d(covar);

  for (int i = 0; i < 100; i++)
    log_info(d.sample());
}
