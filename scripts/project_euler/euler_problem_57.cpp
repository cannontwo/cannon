#include <iostream>
#include <cassert>
#include <cmath>

#include <cannon/log/registry.hpp>
#include <cannon/math/digits.hpp>
#include <cannon/math/bignum.hpp>

using namespace cannon::log;
using namespace cannon::math;

/*!
 * It is possible to show that the square root of two can be expressed as an infinite continued fraction.
 *
 *    \sqrt{2} = 1 + \frac{1}{2 + \frac{1}{2 + \frac{1}{2 + \ldots}}}
 *
 * By expanding this for the first four iterations, we get:
 * 
 *    3/2 = 1.5
 *    7/5 = 1.4
 *    17/12 = 1.41666...
 *    41/29 = 1.41379...
 *
 * The next three expansions are 99/70, 239/169, and 577/408, but the eighth
 * expansion, 1393/985, is the first example where the number of digits in the
 * numerator exceeds the number of digits in the denominator. 
 *
 * In the first one-thousand expansions, how many fractions contain a numerator
 * with more digits than the denominator?
 */

std::pair<BigUnsigned, BigUnsigned>
compute_next_expansion(const BigUnsigned &num, const BigUnsigned &denom) {
  BigUnsigned new_denom(num + denom);
  BigUnsigned new_num(new_denom + denom);

  return std::make_pair(new_num, new_denom);
}

unsigned int compute_num_large_denom_expansions(unsigned int upper) {
  BigUnsigned num = 1;
  BigUnsigned denom = 1;

  unsigned int num_greater = 0;
  for (unsigned int i = 1; i < 1000; ++i) {
    auto [new_num, new_denom] = compute_next_expansion(num, denom);

    if (new_num.size() > new_denom.size())
      ++num_greater;

    num = new_num;
    denom = new_denom;

    log_info("Expansion", i, ":", num, "/", denom);
  }

  return num_greater;
}

int main(int argc, char **argv) {
  std::cout << compute_num_large_denom_expansions(1000) << std::endl;
}
