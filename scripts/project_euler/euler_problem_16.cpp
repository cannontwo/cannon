#include <iostream>

#include <cannon/math/bignum.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::math;
using namespace cannon::log;

/*!
 * 2^15 = 32768 and the sum of its digits is 3 + 2 + 7 + 6 + 8 = 26.
 *
 * What is the sum of the digits of the number 2^1000?
 */

unsigned long get_power_digit_sum() {
  BigUnsigned two_powers("2");

  for (unsigned i = 1; i < 1000; i++) {
    two_powers = two_powers + two_powers;
  }

  log_info("2^1000 =", two_powers);

  unsigned long sum = 0;
  auto digits = two_powers.get_digits();
  for (auto d : digits)
    sum += d;

  return sum;
}

int main(int argc, char** argv) {
  std::cout << get_power_digit_sum() << std::endl;
}
