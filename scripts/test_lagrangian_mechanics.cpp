#include <cmath>

#include <cannon/physics/lagrangian_mechanics.hpp>
#include <cannon/log/registry.hpp>
#include <cannon/ml/bfgs.hpp>

using namespace cannon::physics;
using namespace cannon::log;
using namespace cannon::ml;

double L(const LocalTuple& local) { 
  assert(local.coordinates.size() == 1);

  double m = 1.0;
  double k = 1.0;

  return (1.0 / 2.0) * m * local.velocities.squaredNorm() - 
    (1.0 / 2.0) * k * local.coordinates.squaredNorm();
}

double f(const VectorXd& x) {
  Path q(1, x);

  return compute_path_action(L, q);
}

VectorXd f_grad(const VectorXd& x) {
  VectorXd ret = VectorXd::Zero(x.size());
  Path q(1, x);

  // Time variables
  ret[0] = 0.0;
  ret[1] = 0.0;

  double time_delta = (q.t1 - q.t0) / (double)(q.waypoints.size() - 1);

  for (unsigned int i = 0; i < x.size() - 2; i++) {
    ret[i+2] = L(q.get_local_tuple(q.t0 + i * time_delta));
  }

  return ret;
}

int main() {
  VectorXd start(1);
  start[0] = 1;

  VectorXd end(1);
  end[0] = 0;

  Path q(0.0, M_PI/2.0, start, end, 10);
  log_info("Initial action is", compute_path_action(L, q));

  BFGSOptimizer opt(f, f_grad);
  OptimizationResult result = opt.optimize(q.to_vec());

  log_info(result);

  Path q1(1, result.solution);
  log_info("Action after optimization is", compute_path_action(L, q1));

  // TODO This doesn't currently work because the gradient is too finicky.
  // Should work with compass/pattern search, though
}
