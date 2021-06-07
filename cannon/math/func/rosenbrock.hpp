#ifndef CANNON_MATH_FUNC_ROSENBROCK_H
#define CANNON_MATH_FUNC_ROSENBROCK_H 

/*!
 * \file cannon/math/func/rosenbrock.hpp
 * \brief File containing Rosenbrock function and gradient definitions.
 */

#include <Eigen/Dense>

using namespace Eigen;

namespace cannon {
  namespace math {

    /*!
     * \brief Compute the value of the Rosenbrock function at the input point.
     * See https://en.wikipedia.org/wiki/Rosenbrock_function
     *
     * \param x The point at which to evaluate the Rosenbrock function.
     *
     * \returns The value of the Rosenbrock function at the input point.
     */
    double rosenbrock(const Vector2d& x);

    /*!
     * \brief Compute the gradient of the Rosenbrock function at the input point.
     *
     * \param x The point at which to compute the gradient.
     *
     * \returns Rosenbrock function gradient at the input point.
     */
    Vector2d rosenbrock_grad(const Vector2d& x);

  } // namespace math
} // namespace cannon


#endif /* ifndef CANNON_MATH_FUNC_ROSENBROCK_H */
