#ifndef CANNON_NEARLY_EQUAL_H

/*!
 * \file cannon/math/nearly_equal.hpp
 * \brief File containing utility functions for approximate equality comparison
 * of floating point numbers.
 */

#include <cmath>
#include <limits>
#include <type_traits>
#include <iostream>
#include <iomanip>

namespace cannon {
  namespace math {

    /*!
     * \brief Compute whether two floating point numbers are nearly equal with
     * respect to a tolerance parameter, and taking into account nonlinear
     * floating-point mesh.
     *
     * Adapted from https://floating-point-gui.de/errors/comparison/
     *
     * \param x First number to compare
     * \param y Second number to compare
     * \param epsilon Tolerance parameter
     *
     * \returns Whether the input floats are nearly equal.
     */
    template<class T>
    typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
    nearly_equal(const T& x, const T& y, const T epsilon) {
      const T diff = std::fabs(x - y);
      const T abs_x = std::fabs(x);
      const T abs_y = std::fabs(y);

      if (x == y) 
        return true;
      else if (x == 0 || y == 0 || (abs_x + abs_y < std::numeric_limits<T>::min())) {
        return diff < (epsilon * std::numeric_limits<T>::min());
      } else 
        return diff / std::min((abs_x + abs_y), std::numeric_limits<T>::max()) < epsilon;
    }

    /*!
     * \brief Compute whether two floating point numbers are nearly equal,
     * taking into account nonlinear floating-point mesh.
     *
     * Adapted from https://floating-point-gui.de/errors/comparison/
     *
     * \param x First number to compare
     * \param y Second number to compare
     *
     * \returns Whether the input floats are nearly equal.
     */
    template<class T>
    typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
    nearly_equal(const T& x, const T& y) {
      return nearly_equal(x, y, {0.00001});
    }

  } // namespace math
} // namespace cannon

#define CANNON_NEARLY_EQUAL_H value
#endif /* ifndef CANNON_NEARLY_EQUAL_H */

