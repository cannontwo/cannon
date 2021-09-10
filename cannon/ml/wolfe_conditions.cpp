#include <cannon/ml/wolfe_conditions.hpp>

#include <cannon/log/registry.hpp>

using namespace cannon::ml;
using namespace cannon::log;

bool cannon::ml::sufficient_decrease_condition(RealFunc f, MultiFunc f_grad, const
    VectorXd& x, double step, const VectorXd& direction, double c_1) {

  assert(x.size() == direction.size());
  assert(0 < c_1);
  assert(c_1 < 1);

  double lhs = f(x + (step * direction));
  double rhs = f(x) + c_1 * step * (f_grad(x).dot(direction));

  return lhs <= rhs;
}

// *Strong* Wolfe conditions
bool cannon::ml::curvature_condition(RealFunc /*f*/, MultiFunc f_grad, const
    VectorXd& x, double step, const VectorXd& direction, double c_2) {

  assert(x.size() == direction.size());
  assert(0 < c_2);
  assert(c_2 < 1);

  double lhs = std::fabs(f_grad(x + step * direction).dot(direction));
  double rhs = c_2 * std::fabs(f_grad(x).dot(direction));

  return lhs <= rhs;
}

bool cannon::ml::wolfe_conditions(RealFunc f, MultiFunc f_grad, const
    VectorXd& x, double step, const VectorXd& direction, double c_1, 
    double c_2) {

  return sufficient_decrease_condition(f, f_grad, x, step, direction, c_1) &&
    curvature_condition(f, f_grad, x, step, direction, c_2);
}

// Returns optimal step
double cannon::ml::line_search_zoom(RealFunc f, MultiFunc f_grad, const
    VectorXd& x, const VectorXd& direction, double start_low, double
    start_high, double c_1, double c_2, unsigned int iterations) {

  double current_low = start_low;
  double current_high = start_high;

  double psi_0 = f(x);
  double d_psi_0 = f_grad(x).dot(direction);

  for (unsigned int i = 0; i < iterations; i++) {
    // TODO This is bisection search, but should also be replaced by cubic
    // interpolation
    double a = (current_low + current_high) / 2.0; 

    double psi_i = f(x + a * direction);
    double d_psi_i = f_grad(x + a * direction).dot(direction);

    if (psi_i > (psi_0 + c_1 * a * d_psi_0) || 
        psi_i >= f(x + current_low * direction)) {
      current_high = a;
    } else {
      if (std::fabs(d_psi_i) <= -c_2 * d_psi_0)
        return a;

      if (d_psi_i * (current_high - current_low) >= 0)
        current_high = current_low;

      current_low = a;
    }
  }

  return current_low;
}

// Returns optimal step
double cannon::ml::wolfe_condition_line_search(RealFunc f, MultiFunc f_grad,
    const VectorXd& x, const VectorXd& direction, double step_1, double c_1,
    double c_2, unsigned int iterations) {

  assert(c_1 < c_2);

  if (wolfe_conditions(f, f_grad, x, step_1, direction, c_1, c_2)) 
    return step_1;

  double step_0 = 0.0;
  double step_max = 10 * step_1;
  double current_step = step_1;

  double psi_0 = f(x);
  double d_psi_0 = f_grad(x).dot(direction);

  // Linear step increase
  double rho = (step_max - step_1) / (double)iterations;

  // 'direction' is a descent direction for f
  assert(d_psi_0 < 0);

  double last_step = step_0;
  double last_psi_i = psi_0;

  for (unsigned int i = 0; i < iterations; i++) {
    double psi_i = f(x + current_step * direction);
    double d_psi_i = f_grad(x + current_step * direction).dot(direction);

    if ((psi_i > psi_0 + (c_1 * current_step * d_psi_0)) ||
        ((psi_i >= last_psi_i) && (i > 0))) {
      log_info("Returning on first if statement");
      return line_search_zoom(f, f_grad, x, direction, last_step,
          current_step, c_1, c_2);
    }

    if (std::fabs(d_psi_i) <= -c_2 * d_psi_0) {
      return current_step;
    }

    if (d_psi_i >= 0) {
      return line_search_zoom(f, f_grad, x, direction, current_step,
          last_step, c_1, c_2);
    }

    last_step = current_step;
    last_psi_i = psi_i;

    // TODO Replace with cubic interpolation
    current_step = current_step + rho;
  }

  log_info("Maximum iterations for line search reached");
  return current_step;
}
