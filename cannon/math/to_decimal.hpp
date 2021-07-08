#ifndef CANNON_MATH_TO_DECIMAL_H
#define CANNON_MATH_TO_DECIMAL_H 

/*!
 * \file cannon/math/to_decimal.hpp
 * \brief File containing utilities for working with decimal representations of
 * rational numbers.
 */

#include <vector>
#include <utility>

namespace cannon {
  namespace math {

    /*!
     * \brief Compute the exact decimal representation of the reciprocal of the
     * input integer. All reciprocals are between 0 and 1, exclusive.
     *
     * \param x The integer to compute reciprocal of. Should be >= 2.
     *
     * \returns A pair of vectors in which the first is a vector of non-repeating
     * digits and the second is a vector of repeating digits.
     */
    std::pair<std::vector<unsigned int>, std::vector<unsigned int>>
    compute_decimal_reciprocal(unsigned int x);

    /*!
     * \brief Print the exact decimal representation of the reciprocal of the
     * input integer, in the format
     *
     *   0.x(y)
     *
     * where x is the non-repeating digits of the decimal and y is the
     * recurring digits.
     *
     * \param x The number to print the decimal reciprocal of. 
     */
    void print_decimal_reciprocal(unsigned int x);

  }
}

#endif /* ifndef CANNON_MATH_TO_DECIMAL_H */

