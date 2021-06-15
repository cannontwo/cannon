#include <iostream>
#include <set>

#include <Eigen/Core>

#include <cannon/math/primes.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::math;
using namespace cannon::log;

/*!
 * The sequence of triangle numbers is generated by adding the natural numbers. So the 7th triangle number would be 1 + 2 + 3 + 4 + 5 + 6 + 7 = 28. The first ten terms would be
 *
 *     1, 3, 6, 10, 15, 21, 28, 36, 45, 55
 *
 * Let us list the factors of the first seven triangle numbers:
 *
 *     1: 1
 *     3: 1, 3
 *     6: 1, 2, 3, 6
 *     10: 1, 2, 5, 10
 *     15: 1, 3, 5, 15
 *     21: 1, 3, 7, 21
 *     28: 1, 2, 4, 7, 14, 28
 *
 * We can see that 28 is the first triangle number to have over five divisors. 
 *
 * What is the value of the first triangle number to have over five hundred divisors?
 */

unsigned int get_num_factors(unsigned long n) {
  auto prime_factors = get_prime_factorization(n);
  unsigned int num_factors = 1;

  for (auto it = prime_factors.begin(); it != prime_factors.end(); ) {
    unsigned long factor = *it;

    // Given a prime factorization of the input number, all possible factors
    // are all possible products of the factors
    num_factors *= prime_factors.count(factor) + 1;

    // Go to next unique entry
    do {
      ++it;
    } while (it != prime_factors.end() && *it == factor);
  }

  return num_factors;
}

unsigned long find_triangle_number_with_divisors(unsigned int num_divisors) {
  unsigned long prime_upper = 100;
  auto primes = get_primes_up_to(prime_upper);

  unsigned int n = 1;
  while (true) {
    unsigned int triangle = n * (n + 1) / 2;

    unsigned int n_factors = get_num_factors(triangle);

    if (n_factors > num_divisors) {
      return triangle;
    }

    ++n;
  }

}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "This script takes only one argument: the number of divisors to search for."  << std::endl;
    return 1;
  }

  std::cout << find_triangle_number_with_divisors(std::stoul(argv[1])) << std::endl;
}