#include <iostream>
#include <cassert>
#include <cmath>

#include <cannon/log/registry.hpp>
#include <cannon/math/primes.hpp>

using namespace cannon::log;
using namespace cannon::math;

/*!
 * The first two consecutive numbers to have two distinct prime factors are:
 *
 *   14 = 2 * 7
 *   15 = 3 * 5
 *
 * The first three consecutive numbers to have three distinct prime factors are:
 *
 *   644 = 2^2 * 7 * 23
 *   645 = 3 * 5 * 43
 *   646 = 2 * 17 * 19
 *
 * Find the first four consecutive integers to have four distinct prime factors
 * each. What is the first of these numbers?
 */

bool has_four_distinct_prime_factors(unsigned int x) {
  auto factors = get_prime_factorization(x);
  unsigned int num_factors = 0;

  for (auto it = factors.begin(); it != factors.end();) {
    unsigned int factor = *it;
    ++num_factors;
    
    do {
      ++it;
    } while (it != factors.end() && *it == factor);
  }

  return num_factors == 4;
}

unsigned int find_consecutive_four_prime_factors() {
  unsigned int i = 2;

  while (true) {
    if (has_four_distinct_prime_factors(i)) {
      if (has_four_distinct_prime_factors(i+1) &&
          has_four_distinct_prime_factors(i+2) &&
          has_four_distinct_prime_factors(i+3))
        return i;
    }

    ++i;
  }
}

int main(int argc, char **argv) {
  std::cout << find_consecutive_four_prime_factors() << std::endl;
}
