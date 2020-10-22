#include <Eigen/Dense>

#include <cannon/ml/rls.hpp>
#include <cannon/log/registry.hpp>
#include <cannon/math/nearly_equal.hpp>

using namespace Eigen;

using namespace cannon::ml;
using namespace cannon::log;
using namespace cannon::math;

int main() {
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
    assert((f.predict(tmp_x) - tmp_y).norm() < 0.01);
  }

  f.reset();
  MatrixXd t;
  VectorXd i;
  std::tie(t, i) = f.get_identified_mats();

  assert(t(0,0) == 0.0);
  assert(i[0] == 0.0);
}
