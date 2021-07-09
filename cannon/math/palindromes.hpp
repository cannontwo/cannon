#ifndef CANNON_MATH_PALINDROMES_H
#define CANNON_MATH_PALINDROMES_H

/*!
 * \file cannon/math/palindromes.hpp
 * \brief File containing utilities for working with palindromic numbers in
 * various bases.
 */

#include <vector>

namespace cannon {
  namespace math {

    /*!
     * \brief Check whether the input number, represented as a vector of digits
     * in some basis, is a palindrome. 
     *
     * \sa cannon/math/digits.hpp
     *
     * \param digits The digits of the number to check for being a palindrome.
     *
     * \returns Whether the input digits are palindromic.
     */
    bool is_palindrome(const std::vector<unsigned int>& digits);

    /*!
     * \brief Generate palindromes of exactly the input length in the given basis.
     *
     * \param length Length of palindromes to generate.
     * \param base Base in which to generate palindromes.
     * \param outer Whether this is the first call of this function in a
     * recursive stack. Used to prevent leading zeros.
     *
     * \returns A vector of palindromes of the input length.
     */
    std::vector<unsigned int> generate_palindromes_of_length(unsigned int length,
                                                             unsigned int base=10,
                                                             bool outer = true);

    /*!
     * \brief Generate all palindromes with length up to the input maximum in a
     * particular base.
     *
     * \param max_length Maximum length of palindromes to generate.
     * \param base Base in which to generate palindromes.
     *
     * \returns A vector of all palindromes up to the input length.
     */
    std::vector<unsigned int> generate_palindromes(unsigned int max_length,
                                                   unsigned int base = 10);

  } // namespace math
} // namespace cannon


#endif /* ifndef CANNON_MATH_PALINDROMES_H */
