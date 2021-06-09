#include <iostream>

#include <cannon/math/primes.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::math;
using namespace cannon::log;

/*!
 * The prime factors of 13195 are 5, 7, 13, and 29.
 *
 * What is the largest prime factor of the number 600851475143.
 */

int main(int argc, char** argv) {
  if (argc != 2) {
    log_error("This script takes one argument, which is the number to find the "
              "largest prime factor of.");
    return 1;
  }

  std::cout << get_largest_prime_factor(std::stol(argv[1])) << std::endl;
}
