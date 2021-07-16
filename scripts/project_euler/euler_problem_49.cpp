#include <iostream>
#include <cassert>
#include <cmath>

#include <cannon/log/registry.hpp>
#include <cannon/math/primes.hpp>

using namespace cannon::log;
using namespace cannon::math;

/*!
 * The arithmetic sequence, 1487, 4817, 8147, in which each of the terms
 * increases by 3330, is unusual in two ways: (i) each of the three terms are
 * prime, and (ii) each of the 4-digit numbers are permutations of one another.
 *
 * There are no arithmetic sequences made up of three 1-, 2-, or 3-digit primes,
 * exhibiting this property, but there is one other 4-digit increasing sequence.
 *
 * What 12-digit number do you form by concatenating the three terms in this
 * sequence?
 */

unsigned int get_digit(unsigned int x, unsigned int place) {
  unsigned int trunc = x % static_cast<unsigned int>(std::pow(10, place+1));
  unsigned int digit = trunc / std::pow(10, place);

  return digit;
}

bool are_permutations(unsigned int x, unsigned int y, unsigned int z) {
  if (std::floor(std::log10(x)) != std::floor(std::log10(y)) ||
      std::floor(std::log10(x)) != std::floor(std::log10(z)))
    return false;

  unsigned int num_digits =
      static_cast<unsigned int>(std::floor(std::log10(x))) + 1;

  std::vector<bool> y_taken(num_digits, false);
  std::vector<bool> z_taken(num_digits, false);
  while (x != 0) {
    unsigned int digit = x % 10;

    bool y_found = false;
    bool z_found = false;
    for (unsigned int i = 0; i < num_digits; ++i) {
      if (!y_found && !y_taken[i] && get_digit(y, i) == digit) {
        y_found = true;
        y_taken[i] = true;
      }

      if (!z_found && !z_taken[i] && get_digit(z, i) == digit) {
        z_found = true;
        z_taken[i] = true;
      }
    }

    if (!y_found || !z_found)
      return false;

    x /= 10;
  }

  return true;
}

std::vector<std::vector<unsigned int>> find_prime_permutation_sequences() {
  auto primes = get_primes_up_to(10000u);
  std::vector<std::vector<unsigned int>> ret_vec;

  for (unsigned int i = 0; i < primes.size(); ++i) {
    if (primes[i] < 1000)
      continue;

    for (unsigned int j = i+1; j < primes.size(); ++j) {
      unsigned int diff = primes[j] - primes[i];

      // No more sequences starting with primes[i] are possible
      if (primes[j] + diff > 9999)
        break;
      else if (is_prime(primes[j] + diff) &&
               are_permutations(primes[i], primes[j], primes[j] + diff)) {
        std::vector<unsigned int> seq;
        seq.push_back(primes[i]);
        seq.push_back(primes[j]);
        seq.push_back(primes[j] + diff);
        ret_vec.push_back(seq);
      }
    }
  }

  return ret_vec;
}

int main(int argc, char **argv) {
  auto sequences = find_prime_permutation_sequences();

  for (auto& seq : sequences) {
    std::cout << "Found sequence:" << std::endl;

    for (auto t : seq) {
      std::cout << "\t" << t << std::endl;
    }
  }
}
