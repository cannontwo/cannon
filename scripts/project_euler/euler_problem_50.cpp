#include <iostream>
#include <cassert>
#include <cmath>

#include <cannon/log/registry.hpp>
#include <cannon/math/primes.hpp>

using namespace cannon::log;
using namespace cannon::math;

/*!
 * The prime 41 can be written as the sum of six consecutive primes:
 *   
 *   41 = 2 + 3 + 5 + 7 + 11 + 13
 *
 * This is the longest sum of consecutive primes that adds to a prime below
 * one-hundred. 
 *
 * The longest sum of consecutive primes below one-thousand that adds to a
 * prime contains 21 terms and is equal to 953. 
 *
 * Which prime, below one-million, can be written as the sum of the most
 * consecutive primes?
 */

unsigned int compute_largest_consecutive_prime_sum_length(unsigned int upper) {
  auto primes = get_primes_up_to(upper);

  unsigned int longest_seq = 0;
  unsigned int longest_sum = 0;
  unsigned int lower = 0;

  // Since the primes have to be consecutive, we can exit early as soon as our
  // lower bound cannot lead to a longer sequence.
  while (primes[lower]*longest_seq < upper &&
         lower + longest_seq < primes.size()) {
    unsigned int sum = 0;

    for (unsigned int i = lower; i < primes.size(); ++i) {
      sum += primes[i];

      if (sum > upper)
        break;

      if ((i - lower + 1) > longest_seq && is_prime(sum)) {
        log_info("Found longer consecutive prime sequence of length", longest_seq);
        longest_seq = i - lower + 1;
        longest_sum = sum;
      }
    }

    ++lower;
  }

  return longest_sum;
}

int main(int argc, char **argv) {
  std::cout << compute_largest_consecutive_prime_sum_length(1000000) << std::endl;
}
