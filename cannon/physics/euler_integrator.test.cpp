#include <Eigen/Dense>

#include <cannon/physics/euler_integrator.hpp>
#include <cannon/log/registry.hpp>

using namespace Eigen;

using namespace cannon::physics;
using namespace cannon::log;

void linear_system(const VectorXd& x, VectorXd& dxdt, double /*t*/) {
  static MatrixXd A(2, 2);
  A << -1.0, 0.0,
       0.0, -1.0;

  dxdt = A * x;
}

int main() {
  auto e = make_euler_integrator(linear_system, 2, 0.01);
  VectorXd s(2);
  s << 1.0, 0.0;
  e.set_state(s);

  for (int i = 0; i < 1000; i++) {
    e.step();
    log_info("State is now", e.get_state(), "at time", e.get_time());
  }

  assert(e.get_state().norm() < 0.001);
}
