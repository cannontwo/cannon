#include <iostream>
#include <cassert>

#include <cannon/log/registry.hpp>
#include <cannon/math/primes.hpp>

using namespace cannon::log;
using namespace cannon::math;

/*!
 * Euler discovered the remarkable quadratic formula:
 *     
 *   n^2 + n + 41
 *
 * It turns out that the formula will produce 40 primes for the consecutive
 * integer values 0 <= n <= 39. However, when n=40, 40^2 + 40 + 41 = 40(40 + 1)
 * + 41 is divisible by 41, and certainly when n = 41, 41^2 + 41 + 41 is
 * clearly divisible by 41. 
 *
 * The incredible formula n^2 - 79n + 1601 was discovered, which produces 80
 * primes for the consecutive values 0 <= n <= 79. The product of coefficients,
 * -79 and 1601, is -126479. 
 *
 * Considering quadratics of the form:
 *   
 *   n^2 + an + b, where |a| < 1000 and |b| <= 1000
 *
 * Find the product of the coefficients, a and b, for the quadratic expression
 * that produces the maximum number of primes for consecutive values of n,
 * starting with n = 0.
 */

std::pair<int, unsigned int>
compute_max_primes_linear_coef(int b, int a_abs_max) {
  int max_primes_a = -a_abs_max;
  unsigned int max_primes = 0;

  // a + b + 1 should never be negative
  for (int a = -b+1; a < a_abs_max; ++a) {

    // Assuming b is prime, only consider longer runs of primes if the
    // quadratic is prime for n=1
    if (is_prime(a + b + 1)) {

      int n = 0; 
      while (n*n + a*n + b > 0 && is_prime(n*n + a*n + b)) {
        ++n;
      }

      if (n - 1 > max_primes) {
        max_primes = n - 1;
        max_primes_a = a;
      }
    }
  }

  return std::make_pair(max_primes_a, max_primes);
}

long compute_maximum_prime_coef_product(unsigned int a_max, unsigned int b_max) {

  // b can only be positive primes, since quadratic has to produce a prime for n=0.
  std::vector<int> b_vals = get_primes_up_to(static_cast<int>(b_max));
  unsigned long max_product = 1;
  unsigned int max_cons_primes = 1;
  
  for (auto& b : b_vals) {
    auto[a, num_primes] = compute_max_primes_linear_coef(b, a_max);

    if (num_primes == 0) {
      // No a leading to a quadratic generating more primes than for n=0 was
      // found for this b
      continue;
    } else {
      if (num_primes > max_cons_primes) {
        log_info("New max a is", a, ", new max b is", b);
        log_info("Maximum consecutive primes is", num_primes);
        max_cons_primes = num_primes;
        max_product = a * b;
      }
    }
  }

  return max_product;
}

int main(int argc, char** argv) {
  std::cout << compute_maximum_prime_coef_product(1000, 1000) << std::endl;
}
