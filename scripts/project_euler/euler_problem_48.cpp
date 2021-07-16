#include <iostream>
#include <cassert>
#include <cmath>

#include <cannon/log/registry.hpp>

using namespace cannon::log;

/*!
 * The series, 1^1 + 2^2 + 3^3 + ... + 10^10 = 10405071317.
 *
 * Find the last ten digits of the series, 1^1 + 2^2 + 3^3 + ... + 1000^1000.
 */

unsigned long compute_power_sum_last_ten_digits(unsigned int upper) {
  unsigned long sum = 0;

  for (unsigned int i = 1; i <= upper; ++i) {
    unsigned long term = 1;

    for (unsigned int j = 0; j < i; ++j) {
      term *= i;
      term = term % static_cast<unsigned long>(std::pow(10, 11));
    }

    sum += term;
    sum = sum % static_cast<unsigned long>(std::pow(10, 10));
  }

  return sum;
}

int main(int argc, char **argv) {
  std::cout << compute_power_sum_last_ten_digits(1000) << std::endl;
}
