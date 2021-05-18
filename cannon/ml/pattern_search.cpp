#include <cannon/ml/pattern_search.hpp>

#include <cannon/ml/optimization_result.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::ml;
using namespace cannon::log;

//Public Methods
OptimizationResult PatternSearchOptimizer::optimize(const VectorXd& start,
    unsigned int iterations, double eps, double contraction_param, double
    expansion_param, double initial_step) {
  assert(start.size() == dim_);
  assert(eps > 0.0);
  assert(contraction_param > 0.0);
  assert(contraction_param < 1.0);
  assert(expansion_param > 1.0);
  assert(initial_step > 0.0);
  assert(initial_step > eps);

  VectorXd first = VectorXd::Ones(dim_);
  auto direction_set = create_simplex_set_(first);
  VectorXd leading_dir = first;

  VectorXd last_x = start;
  VectorXd x = start;
  double step = initial_step;

  for (unsigned int i = 0; i < iterations; i++) {
    if (step < eps)
      return OptimizationResult(f_(x), x, i, false);

    for (auto& dir : direction_set) {
      if (f_(x + step*dir) < f_(x) - forcing_func_(step)) {
        last_x = x;
        x = last_x + step * dir;
        step = expansion_param * step;

        if (reorient_) {
          VectorXd diff = x - last_x;
          update_leading_dir_(leading_dir, diff);
          direction_set = create_simplex_set_(leading_dir);
        }

        break;
      }
    }

    // No direction decreased f_, so we shrink step
    step = contraction_param * step;

  }
  
  return OptimizationResult(f_(x), x, iterations, true);
}

// Private Methods
std::vector<VectorXd> PatternSearchOptimizer::create_simplex_set_(const VectorXd& first) {
  std::vector<VectorXd> ret_vec;

  ret_vec.push_back(first.normalized());

  for (unsigned int i = 0; i < dim_; i++) {
    VectorXd unit_vec = VectorXd::Zero(dim_);
    unit_vec[i] = 1.0;

    VectorXd unnormalized_vec = first - unit_vec;

    ret_vec.push_back(unnormalized_vec.normalized());
  }

  return ret_vec;
}

double PatternSearchOptimizer::forcing_func_(double t) {
  return forcing_func_param_ * std::pow(t, 2);
}

void PatternSearchOptimizer::update_leading_dir_(VectorXd& leading_dir,
    const VectorXd& diff) {
  leading_dir = reorient_param_*diff + (1.0 - reorient_param_)*leading_dir;

  // Protect against alignment with coordinate axes
  unsigned int num_nonzero = 0;
  for (unsigned int i = 0; i < leading_dir.size(); i++) {
    if (leading_dir[i] != 0.0)
      num_nonzero += 1;
  }

  if (num_nonzero <= 1) {
    leading_dir = VectorXd::Ones(dim_);
  }
}
