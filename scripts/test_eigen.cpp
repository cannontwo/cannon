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

  Matrix3d A;
  A << 1, 2, 3,
       4, 5, 6,
       7, 8, 9;

  Vector3d a = Vector3d::Zero();
  for (unsigned int i = 0; i < 3; i++) {
    a[i] = *(A.row(1).data() + i);
  }
  log_info(a);

}
