#include <iostream>
#include <cassert>
#include <vector>

#include <cannon/log/registry.hpp>
#include <cannon/math/primes.hpp>

using namespace cannon::log;
using namespace cannon::math;

/*!
 * Let d(n) be defined as the sum of proper divisors of n (numbers less than n
 * which divide evenly into n). If d(a) = b and d(b) = a, where a != b, then a
 * and b are an amicable pair and each of a and b are called amicable numbers.
 *
 * For example, the proper divisors of 220 are 1, 2, 4, 5, 10, 11, 20, 22, 44,
 * 55, 110; therefore d(220) = 284. The proper divisors of 284 are 1, 2, 3, 71,
 * 142; so d(284) = 220.
 *
 * Evaluate the sum of all the amicable numbers under 10000. 
 */

unsigned int get_divisor_sum(unsigned int x) {
  if (x == 0)
    return 0;

  auto prime_factors = get_prime_factorization(x);

  unsigned int total_sum = 1;
  for (auto it = prime_factors.begin(); it != prime_factors.end();) {
    unsigned long factor = *it;
    unsigned int prime_power_sum = 0;

    // See https://www2.math.upenn.edu/~deturck/m170/wk3/lecture/sumdiv.html
    for (unsigned int i = 0; i < prime_factors.count(factor) + 1; ++i) {
      prime_power_sum += std::pow(factor, i); 
    }

    total_sum *= prime_power_sum;

    do {
      ++it;
    } while (it != prime_factors.end() && *it == factor);
  }

  return total_sum - x;
}

unsigned int get_sum_amicable_below(unsigned int upper) {
  std::vector<unsigned int> divisor_sums;
  std::vector<bool> amicable;

  for (unsigned int i = 0; i < upper; ++i) {
    unsigned int s = get_divisor_sum(i);

    divisor_sums.push_back(s);
    amicable.push_back(false);

    if (s < i) {
      if (divisor_sums[s] == i) {
        log_info("Found amicable pair: (", s, ",", i, ")");
        amicable[s] = true;
        amicable[i] = true;
      }
    } 
  }

  unsigned int total_sum = 0;
  for (unsigned int i = 0; i < divisor_sums.size(); ++i) {
    if (amicable[i])
      total_sum += i;
  }

  return total_sum;
}

int main(int argc, char** argv) {
  std::cout << get_sum_amicable_below(10000) << std::endl;
}
