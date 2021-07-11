#include <iostream>
#include <cassert>
#include <cmath>

#include <cannon/log/registry.hpp>
#include <cannon/math/primes.hpp>

using namespace cannon::log;
using namespace cannon::math;

/*!
 * We shall say that an n-digit number is pandigital if it makes use of all the
 * digits 1 to n exactly once. For example, 2143 is a 4-digit pandigital and is
 * also prime.
 *
 * What is the largest n-digit pandigital prime that exists?
 */

std::vector<unsigned int>
generate_permutations(const std::vector<unsigned int> options) {
  std::vector<unsigned int> ret_vec;

  if (options.size() == 0) {
    std::vector<unsigned int> zero(1, 0);
    return zero;
  }

  for (const auto& x : options) {
    std::vector<unsigned int> sub_options;
    for (const auto& y : options) {
      if (y != x)
        sub_options.push_back(y);
    }

    auto sub_perms = generate_permutations(sub_options);
    for (auto& perm : sub_perms) {
      perm *= 10;
      perm += x;

      ret_vec.push_back(perm);
    }
  }

  return ret_vec;
}

std::vector<unsigned int> generate_pandigitals(unsigned int n) {
  std::vector<unsigned int> ret_vec;

  std::vector<unsigned int> digits;
  for (unsigned int i = 1; i <= n; ++i)
    digits.push_back(i);

  return generate_permutations(digits);
}

unsigned int compute_largest_pandigital_prime() {
  unsigned int largest = 0;
  for (int i = 9; i > 3; --i) {
    auto pans = generate_pandigitals(i);

    for (auto& x : pans) {
      if (is_prime(x)) {
        log_info("Found prime:", x);

        if (x > largest)
          largest = x;
      }
    }

    if (largest != 0)
      return largest;
  } 

  return largest;
}

int main(int argc, char** argv) {
  std::cout << compute_largest_pandigital_prime() << std::endl;
}
