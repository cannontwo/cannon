#ifndef CANNON_MATH_ROOTFINDING_H
#define CANNON_MATH_ROOTFINDING_H

/*!
 * \file cannon/math/rootfinding.hpp
 * \brief File containing various rootfinding utilities for real-valued
 * functions.
 */

#include <functional>

namespace cannon {
  namespace math {

    /*!
     * \brief Compute an approximate root of the input function using the
     * bisection method. See https://en.wikipedia.org/wiki/Bisection_method
     *
     * \param f The function to find a root of
     * \param a Lower bound of interval
     * \param b Upper bound of interval
     * \param iters Maximum number of iterations
     *
     * \returns An approximate root of the function in the interval [a, b].
     */
    double bisection_method(std::function<double(double)> f, double a, double b,
                            unsigned int iters = 100);
  }
}

#endif /* ifndef CANNON_MATH_ROOTFINDING_H */
