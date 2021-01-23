#include <cannon/ml/bfgs.hpp>

using namespace cannon::ml;

OptimizationResult BFGSOptimizer::optimize(const VectorXd& start, double eps,
    unsigned int iterations) {

  // TODO May want to apply scaling to this at some point
  MatrixXd inv_hessian_approx = MatrixXd::Identity(start.size(), start.size());
  MatrixXd I = MatrixXd::Identity(start.size(), start.size());
  VectorXd x = start;

  for (unsigned int i = 0; i < iterations; i++) {
    if (f_grad_(x).norm() <= eps)
      return OptimizationResult(f_(x), x, i, false);

    VectorXd direction = -inv_hessian_approx * f_grad_(x);
    double step = wolfe_condition_line_search(f_, f_grad_, x, direction);

    VectorXd next_x = x + step * direction;
    VectorXd s = next_x - x;
    VectorXd y = f_grad_(next_x) - f_grad_(x);
    double rho = 1.0 / y.dot(s);

    MatrixXd next_inv_hessian_approx = (I - (rho * (s * y.transpose()))) * 
                                       inv_hessian_approx *
                                       (I - (rho * (y * s.transpose()))) + 
                                       rho * (s * s.transpose());

    inv_hessian_approx = next_inv_hessian_approx;
    x = next_x;
  }

  return OptimizationResult(f_(x), x, iterations, true);
}
