#ifndef CANNON_MATH_FUNC_ROSENBROCK_H
#define CANNON_MATH_FUNC_ROSENBROCK_H 

#include <Eigen/Dense>

using namespace Eigen;

namespace cannon {
  namespace math {

    double rosenbrock(const Vector2d& x);

    Vector2d rosenbrock_grad(const Vector2d& x);

  } // namespace math
} // namespace cannon


#endif /* ifndef CANNON_MATH_FUNC_ROSENBROCK_H */
