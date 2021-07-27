#include <iostream>
#include <cassert>
#include <cmath>

#include <cannon/log/registry.hpp>
#include <cannon/math/bignum.hpp>

using namespace cannon::log;
using namespace cannon::math;

/*!
 * A googol (10^100) is a massive number: one followed by one-hundred zeros;
 * 100^100 is almost unimaginably large: one followed by two-hundred zeros.
 * Despite their size, the sum of the digits in each number is only 1. 
 *
 * Considering natural numbers of the form a^b, where a, b < 100, what is the
 * maximum digital sum?
 */

BigUnsigned big_power(unsigned int a, unsigned int b) {
  BigUnsigned ret_num(a);

  for (unsigned int i = 2; i <= b; i++) {
    ret_num *= a;
  }

  return ret_num;
}

unsigned int compute_maximum_power_digit_sum() {
  unsigned int max_digit_sum = 0;

  for (unsigned int a = 1; a < 100; ++a) {
    for (unsigned int b = 1; b < 100; ++b) {
      auto p = big_power(a, b);

      unsigned int p_sum = 0;
      for (auto& d : p.get_digits()) {
        p_sum += d;
      }

      if (p_sum > max_digit_sum)
        max_digit_sum = p_sum;

    }
  }

  return max_digit_sum;
}

int main(int argc, char **argv) {
  std::cout << compute_maximum_power_digit_sum() << std::endl;
}
