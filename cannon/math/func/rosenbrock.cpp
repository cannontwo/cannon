#include <cannon/math/func/rosenbrock.hpp>

using namespace cannon::math;

double cannon::math::rosenbrock(const Vector2d& x) {
  double term1 = 100 * std::pow(x[1] - std::pow(x[0], 2), 2);
  double term2 = std::pow(1 - x[0], 2);
  return term1 + term2;
}

Vector2d cannon::math::rosenbrock_grad(const Vector2d& x) {
  Vector2d ret = Vector2d::Zero();

  ret[0] = (400 * std::pow(x[0], 3)) -
    (400 * x[0] * x[1]) + 
    (2 * x[0]) -
    2;

  ret[1] = 200 * (x[1] - std::pow(x[0], 2));

  return ret;
}
