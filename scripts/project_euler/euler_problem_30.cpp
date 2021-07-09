#include <iostream>
#include <cassert>
#include <cmath>

#include <cannon/log/registry.hpp>

using namespace cannon::log;

/*!
 * Surprisingly there are only three numbers that can be written as the sum of fourth powers of their digits:
 *
 *   1634 = 1^4 + 6^4 + 3^4 + 4^4
 *   8208 = 8^4 + 2^4 + 0^4 + 8^4
 *   9474 = 9^4 + 4^4 + 7^4 + 4^4
 *
 * As 1 = 1^4 is not a sum it is not included.
 *
 * The sum of these numbers is 1634 + 8208 + 9474 = 19316. 
 *
 * Find the sum of all the numbers that can be written as the sum of fifth
 * powers of their digits. 
 */


unsigned int compute_digit_power_sum(unsigned int x, unsigned int power) {
  unsigned int sum = 0;

  while (x != 0) {
    sum += std::pow(x % 10, power);
    x /= 10;
  }

  return sum;
}

std::vector<unsigned int> compute_digit_power_nums(unsigned int power,
                                                   unsigned int upper) {
  std::vector<unsigned int> ret_vec;

  for (unsigned int i = 2; i < upper; ++i) {
    if (i == compute_digit_power_sum(i, power)) {
      log_info("Found number:", i);
      ret_vec.push_back(i);
    }
  }

  return ret_vec;
}

// As 9^5 * 2 has six digits and 9^5 * 10 has six digits, we only need to
// consider numbers up to 999999

int main(int argc, char** argv) {
  auto nums = compute_digit_power_nums(5, 999999);

  unsigned int sum = 0;
  for (auto x : nums)
    sum += x;


  std::cout << sum << std::endl;
}
