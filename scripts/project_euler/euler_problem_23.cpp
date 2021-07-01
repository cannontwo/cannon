#include <iostream>
#include <cassert>

#include <cannon/log/registry.hpp>
#include <cannon/math/divisors.hpp>

using namespace cannon::log;
using namespace cannon::math;

/*!
 * A perfect number is a number for which the sum of its proper divisors is
 * exactly equal to the number. For example, the sum of the proper divisors of
 * 28 would be 1 + 2 + 4 + 7 + 14 = 28, which means that 28 is a perfect
 * number.
 *
 * A number n is called deficient if the sum of its proper divisors is less
 * than n and it is called abundant if this sum exceeds n.
 *
 * As 12 is the smallest abundant number, 1 + 2 + 3 + 4 + 6 = 16, the smallest
 * number that can be written as the sum of two abundant numbers is 24. By
 * mathematical analysis, it can be shown that all integers greater than 28123
 * can be written as the sum of two abundant numbers. However, this upper limit
 * cannot be reduced any further by analysis even though it is known that the
 * greatest number that cannot be expressed as the sum of two abundant numbers
 * is less than this limit.
 *
 * Find the sum of all the positive integers which cannot be written as the sum
 * of two abundant numbers. 
 */

unsigned long compute_non_abundant_sum() {
  unsigned int upper = 28124;

  std::vector<bool> is_abundant(upper, false);
  for (unsigned int i = 0; i < upper; ++i) {
    if (get_divisor_sum(i) > i)
      is_abundant[i] = true;
  }

  std::vector<bool> is_abundant_sum(upper, false);
  for (unsigned int i = 0; i < upper; ++i) {
    if (is_abundant[i]) {
      for (unsigned int j = i; j < upper - i; ++j) {
        if (is_abundant[j]) 
          is_abundant_sum[i + j] = true;
      }
    }
  }

  unsigned long sum = 0;
  for (unsigned int i = 0; i < upper; ++i) {
    if (!is_abundant_sum[i])
        sum += i;
  }

  return sum;
}

int main(int argc, char** argv) {
  std::cout << compute_non_abundant_sum() << std::endl;
}
