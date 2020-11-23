#include <catch2/catch.hpp>

#include <Eigen/Dense>

#include <cannon/ml/rls.hpp>
#include <cannon/log/registry.hpp>
#include <cannon/math/nearly_equal.hpp>

using namespace Eigen;

using namespace cannon::ml;
using namespace cannon::log;
using namespace cannon::math;

TEST_CASE("RLS", "[ml]") {
  // Basic correctness
  RLSFilter f(1, 1);

  VectorXd xs = VectorXd::LinSpaced(10, 0.0, 1.0);
  VectorXd ys = 5.0 * xs;

  log_info("xs is", xs, "ys is", ys);

  for (int i = 0; i < 10; i++) {
    VectorXd tmp_x(1);
    tmp_x << xs[i];
    VectorXd tmp_y(1);
    tmp_y << ys[i];
    f.process_datum(tmp_x, tmp_y);
  }

  for (int i = 0; i < 10; i++) {
    VectorXd tmp_x(1);
    tmp_x << xs[i];
    VectorXd tmp_y(1);
    tmp_y << ys[i];
    log_info("Predicted", f.predict(tmp_x), "for actual", tmp_y);
    REQUIRE((f.predict(tmp_x) - tmp_y).norm() < 0.01);
  }

  f.reset();
  MatrixXd t;
  VectorXd i;
  std::tie(t, i) = f.get_identified_mats();

  REQUIRE(t(0,0) == 0.0);
  REQUIRE(i[0] == 0.0);
}
