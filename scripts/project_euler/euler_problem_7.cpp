#include <iostream>
#include <cmath>

#include <cannon/log/registry.hpp>
#include <cannon/math/primes.hpp>

using namespace cannon::math;
using namespace cannon::log;

/*!
 * By listing the first six prime numbers: 2, 3, 5, 7, 11, and 13, we can see
 * that the 6th prime is 13. 
 *
 * What is the 10,001st prime number?
 */

unsigned int get_nth_prime_number(unsigned int n) {
  unsigned int upper = n;

  while (true) {
    auto primes = get_primes_up_to(upper);

    if (primes.size() >= n) {
      return primes[n-1];
    }

    upper *= 2;
  }
}

int main() {
  std::cout << get_nth_prime_number(10001) << std::endl;
}
