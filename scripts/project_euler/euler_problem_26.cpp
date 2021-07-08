#include <iostream>
#include <cassert>
#include <vector>
#include <sstream>

#include <cannon/log/registry.hpp>
#include <cannon/math/to_decimal.hpp>

using namespace cannon::log;
using namespace cannon::math;

/*!
 * A unit fraction contains 1 in the numerator. The decimal representation of
 * the unit fractions with denominators 2 to 10 are given:
 *
 *   1/2 = 0.5
 *   1/3 = 0.(3)
 *   1/4 = 0.25
 *   1/5 = 0.2
 *   1/6 = 0.1(6)
 *   1/7 = 0.(142857)
 *   1/8 = 0.125
 *   1/9 = 0.(1)
 *   1/10 = 0.1
 *
 * Where 0.1(6) means 0.1666666..., and has a 1-digit recurring cycle. It can
 * be seen that 1/7 has a 6-digit recurring cycle.
 *
 * Find the value of d < 1000 for which 1/d contains the longest recurring
 * cycle in its decimal fraction part.
 */

unsigned int compute_largest_reciprocal_digit_cycle(unsigned int upper) {
  unsigned int max_recurring = 1;
  unsigned int max_recurring_num = 1;

  for (unsigned int i = 2; i < upper; ++i) {
    auto [non_repeating, repeating] = compute_decimal_reciprocal(i);

    // Print number
    print_decimal_reciprocal(i);

    if (repeating.size() > max_recurring) {
      max_recurring = repeating.size();
      max_recurring_num = i;
    }

  }

  return max_recurring_num;
}

int main(int argc, char** argv) {
  std::cout << compute_largest_reciprocal_digit_cycle(1000) << std::endl;
}
