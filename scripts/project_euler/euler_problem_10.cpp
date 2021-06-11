#include <iostream>

#include <cannon/math/primes.hpp>

using namespace cannon::math;

/*!
 * The sum of the primes below 10 is 2 + 3 + 5 + 7 = 17.
 *
 * Find the sum of all the primes below two million.
 */

unsigned long sum_primes_below(unsigned int upper) {
  auto primes = get_primes_up_to(upper);
  unsigned long sum = 0;

  for (auto p : primes)
    sum += p;

  return sum;
}

int main() {
  std::cout << sum_primes_below(2e6) << std::endl;
}
