#include <iostream>
#include <cassert>

#include <cannon/log/registry.hpp>

using namespace cannon::log;

/*!
 * 145 is a curious number, as 1! + 4! + 5! = 1 + 24 + 120 = 145.
 *
 * Find the sum of all numbers which are equal to the sum of the factorial of
 * their digits.
 *
 * Note: As 1! and 2! = 2 are not sums they are not included. 
 */

// As 9! * 8 has only 7 digits, we only need to consider numbers up to 9,999,999.

inline unsigned int factorial(unsigned int x) {
  unsigned int prod = 1;
  for (unsigned int i = 1; i <= x; ++i) {
    prod *= i; 
  }

  return prod;
}

bool is_curious(unsigned int x) {
  unsigned int fact_sum = 0;
  unsigned int tmp = x;
  while (tmp != 0) {
    unsigned int digit = tmp % 10;
    fact_sum += factorial(digit);
    tmp /= 10;
  }

  return fact_sum == x;
}

unsigned int compute_digit_factorial_num_sum() {
  unsigned int sum = 0;

  for (unsigned int i = 10; i < 9999999; ++i) {
    if (is_curious(i)) {
      log_info("Found curious number:", i);
      sum += i;
    }
  }

  return sum;
}

int main(int argc, char** argv) {
  std::cout << compute_digit_factorial_num_sum() << std::endl;
}
