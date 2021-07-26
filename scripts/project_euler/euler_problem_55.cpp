#include <iostream>
#include <cassert>
#include <cmath>
#include <algorithm>

#include <cannon/log/registry.hpp>
#include <cannon/math/palindromes.hpp>
#include <cannon/math/bignum.hpp>

using namespace cannon::log;
using namespace cannon::math;

/*!
 * If we take 47, reverse and add, 47 + 74 = 121, which is palindromic.
 *
 * Not all numbers produce palindromes so quickly. For example,
 *
 *   349 + 943 = 1292,
 *   1292 + 2921 = 4213,
 *   4213 + 3124 = 7337
 *
 * That is, 349 took three iterations to arrive at a palindrome.
 *
 * Although no one has proved it yet, it is thought that some numbers, like 196,
 * never produce a palindrome. A number that never forms a palindrome through
 * the reverse and add process is called a Lychrel number. Due to the
 * theoretical nature of these numbers, and for the purpose of this problem, we
 * shall assume that a number is Lychrel until proven otherwise. In addition you
 * are given that for every number below ten-thousand, it will either (i) become
 * a palindrome in less than fifty iterations, or, (ii) no one, with all the
 * computing power that exists, has managed so far to map it to a palindrome. In
 * fact, 10677 is the first number to be shown to require over fifty iterations
 * before producing a palindrome: 4668731596684224866951378664 (53 iterations,
 * 28-digits).
 *
 * Surprisingly, there are palindromic numbers that are themselves Lychrel
 * numbers; the first example is 4994.
 *
 * How many Lychrel numbers are there below ten-thousand?
 *
 */

bool is_lychrel(const BigUnsigned& x, unsigned int iters) {
  if (iters <= 0)
    return true;
  else {
    auto digits = x.get_digits();
    std::reverse(digits.begin(), digits.end());
    auto x_rev = BigUnsigned(digits);

    log_info(x, "reversed is", x_rev);

    auto next_x = x + x_rev;
    if (is_palindrome(next_x.get_digits()))
      return false;
    else
      return is_lychrel(next_x, iters - 1);
  }
}

bool is_lychrel(unsigned int x) {
  return is_lychrel(x, 50);
}

unsigned int compute_num_lycrel_numbers(unsigned int upper) {
  unsigned int num_lychrel = 0;

  for (unsigned int i = 10; i < upper; ++i) {
    if (is_lychrel(i))
      ++num_lychrel;
  }

  return num_lychrel;
}

int main(int argc, char **argv) {
  std::cout << compute_num_lycrel_numbers(10000) << std::endl;
}
