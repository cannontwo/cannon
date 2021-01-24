#include <cmath>

#include <cannon/physics/lagrangian_mechanics.hpp>
#include <cannon/log/registry.hpp>
#include <cannon/ml/pattern_search.hpp>

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
  VectorXd start(1);
  start[0] = M_PI/2.0;

  VectorXd end(1);
  end[0] = 0;

  q.waypoints[0] = start;
  q.waypoints[q.waypoints.size() - 1] = end;
  q.t0 = 0.0;
  q.t1 = 1.0;

  return compute_path_action(L, q);
}

int main() {
  VectorXd start(1);
  start[0] = M_PI/2.0;

  VectorXd end(1);
  end[0] = 0;

  Path q(0.0, M_PI/2.0, start, end, 10);
  log_info("Initial action is", compute_path_action(L, q));

  PatternSearchOptimizer opt(f, 10, true);
  OptimizationResult result = opt.optimize(q.to_vec());

  log_info(result);

  Path q1(1, result.solution);
  q1.t0 = 0.0;
  q1.t1 = 1.0;
  q1.waypoints[0] = start;
  q1.waypoints[q1.waypoints.size() - 1] = end;
  log_info("Action after optimization is", compute_path_action(L, q1));

  // TODO This doesn't currently work because the gradient is too finicky.
  // Should work with compass/pattern search, though
}
