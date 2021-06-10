#include <iostream>
#include <cmath>

#include <cannon/math/primes.hpp>

using namespace cannon::math;

/*!
 * 2520 is the smallest number that can be divided by each of the numbers from
 * 1 to 10 without any remainder.
 *
 * What is the smallest positive number that is evenly divisible by all the
 * numbers from 1 to 20?
 */

unsigned int get_smallest_multiple(unsigned int upper) {
  unsigned int multiple = 1;
  auto primes = get_primes_up_to(upper);

  // We have to include all prime multiples, as well as squares of primes that
  // fall within the range of interest
  for (unsigned int i = 0; i < primes.size(); i++) {
    unsigned int pow = 1;
    while (std::pow(primes[i], pow) <= upper) {
      if (multiple % static_cast<unsigned int>(std::round(std::pow(primes[i], pow))) != 0) {
        multiple *= primes[i];
      }
      ++pow;
    }
  }

  return multiple;
}

int main() {
  std::cout << get_smallest_multiple(20) << std::endl;
}
