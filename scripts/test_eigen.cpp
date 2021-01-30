#include <Eigen/Dense>

#include <cannon/log/registry.hpp>

using namespace Eigen;

using namespace cannon::log;

int main() {
  Vector3d x = Vector3d::Zero();
  Vector3d y = Vector3d::Zero();

  x[1] = 1.0;
  x[2] = 2.0;

  y[0] = 1.0;
  y[1] = 2.0;
  y[2] = 3.0;

  log_info(x.array() > 0.0);
  log_info((x.array() <= y.array()).all());
  log_info(x.array().max(1));
}
