#include <iostream>
#include <cassert>
#include <cmath>

#include <cannon/log/registry.hpp>
#include <cannon/math/primes.hpp>

using namespace cannon::log;
using namespace cannon::math;

/*!
 * It was proposed by Christian Goldbach that every odd composite number can be
 * written as the sum of a prime and twice a square.
 *
 *   9 = 7 + 2*1^2
 *   15 = 7 + 2*2^2
 *   21 = 3 + 2*3^2
 *   25 = 7 + 2*3^2
 *   27 = 19 + 2*2^2
 *   33 = 31 + 2*1^2
 *
 * It turns out that the conjecture was false.
 *
 * What is the smallest odd composite that cannot be written as the sum of a
 * prime and twice a square?
 */

unsigned int compute_goldbach_counterexample() {
  unsigned int i = 1;

  while (true) {
    i += 2;

    if (is_prime(i))
      continue;

    log_info("Checking", i);

    bool found_decomp = false;
    for (unsigned int j = 1; j < std::sqrt((i+1) / 2); ++j) {
      if (is_prime(i - 2*j*j)) {
        log_info("For composite", i, "found decomposition", i - 2*j*j, " + 2 *", j, "^2");
        found_decomp = true;
        break;
      }
    }

    if (!found_decomp)
      return i;
  }
}

int main(int argc, char **argv) {
  std::cout << compute_goldbach_counterexample() << std::endl;
}
