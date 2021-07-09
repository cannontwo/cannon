#ifndef CANNON_MATH_DIGITS_H
#define CANNON_MATH_DIGITS_H 

/*!
 * \file cannon/math/digits.hpp
 * \brief File containing utilities for working with digit representations of
 * numbers in various bases.
 */

#include <vector>

namespace cannon {
  namespace math {

    /*!
     * \brief Get a vector of digits of the input number in the input base.
     *
     * \param x The number to get digits of.
     * \param base The base to get digits in.
     *
     * \returns A vector of the digits of x, where the digit at index 0 is the
     * digit occupying the "0s place" in the number. In other words, x can be
     * recovered as base^0 * digits[0] + ... + base^(digits.size() - 1) *
     * digits[digits.size() - 1].
     */
    std::vector<unsigned int> get_digits(unsigned int x, unsigned int base = 10);

    /*!
     * \brief Convert a vector of digits in the input base into an unsigned int.
     *
     * \param digits The digits
     * \param base The base that the digits are in
     *
     * \returns An equivalent unsigned int
     */
    unsigned int from_digits(const std::vector<unsigned int> &digits,
                             unsigned int base = 10);

  } // namespace math
} // namespace cannon


#endif /* ifndef CANNON_MATH_DIGITS_H */
