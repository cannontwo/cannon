#include <iostream>
#include <cassert>

#include <cannon/log/registry.hpp>
#include <cannon/math/bignum.hpp>

using namespace cannon::log;
using namespace cannon::math;

/*!
 * n! means n * (n - 1) * ... * 3 * 2 * 1
 *
 * For example, 10! = 10 * 9 * ... * 3 * 2 * 1 = 3628800, and the sum of the
 * digits in the number 10! is 3 + 6 + 2 + 8 + 8 + 0 + 0 = 27. 
 *
 * Find the sum of the digits in the number 100!. 
 */

unsigned int get_factorial_digit_sum(unsigned int x) {
  BigUnsigned fact("1");

  for (unsigned int i = x; i > 0; --i) {
    fact = fact * i;
  }

  log_info(x, "factorial is", fact);

  unsigned int ret_sum = 0;
  auto fact_digits = fact.get_digits();
  for (auto& d : fact_digits) {
    ret_sum += d;
  }

  return ret_sum;
}

int main(int argc, char** argv) {
  std::cout << get_factorial_digit_sum(100) << std::endl;
}
