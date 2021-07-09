#include <iostream>
#include <cassert>
#include <cmath>
#include <algorithm>

#include <cannon/log/registry.hpp>
#include <cannon/math/primes.hpp>
#include <cannon/math/digits.hpp>

using namespace cannon::log;
using namespace cannon::math;

/*!
 * The number 3797 has an interesting property. Being prime itself, it is
 * possible to continuously remove digits from left o right, and remain prime at
 * each stage: 3797, 797, 97, and 7. Similarly, we can work from right to left:
 * 3797, 379, 37, and 3.
 *
 * Find the sum of the only eleven primes that are both truncatable from left to
 * right and right to left.
 *
 * Note: 2, 3, 5, and 7 are not considered to be truncatable primes.
 */

bool is_right_truncatable(unsigned int x) {
  if (!is_prime(x))
    return false;

  while (x != 0) {
    if (!is_prime(x))
      return false;

    x /= 10;
  }

  return true;
}

bool is_left_truncatable(unsigned int x) {

  while (x != 0) {
    if (!is_prime(x))
      return false;

    unsigned int first_digit_pow = std::floor(std::log10(x));
    unsigned int first_digit = x / std::pow(10, first_digit_pow);

    x -= first_digit * std::pow(10, first_digit_pow);
  }

  return true;
}

bool is_truncatable(unsigned int x) {
  return is_left_truncatable(x) && is_right_truncatable(x);
}

unsigned int compute_truncatable_primes_sum() {
  unsigned int sum = 0;
  unsigned int num_truncatable = 0;

  unsigned int upper = 1000;
  auto primes = get_primes_up_to(upper);
  
  // Skipping 2-7
  unsigned int i = 4;
  while (num_truncatable < 11) {
    for (; i < primes.size(); ++i) {
      if (is_truncatable(primes[i])) {
        log_info("Found truncatable prime:", primes[i]);
        sum += primes[i];
        ++num_truncatable;
      }
    }
    
    upper *= 2;
    primes = get_primes_up_to(upper);
  }

  return sum;
}

int main(int argc, char** argv) {
  std::cout << compute_truncatable_primes_sum() << std::endl;
}
