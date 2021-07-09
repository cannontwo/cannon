#include <iostream>
#include <cassert>

#include <cannon/log/registry.hpp>
#include <cannon/math/primes.hpp>

using namespace cannon::log;
using namespace cannon::math;

/*!
 * The number, 197, is called a circular prime because all rotations of the
 * digits: 197, 971, 719, are themselves prime.
 *
 * There are thirteen such primes below 100: 2, 3, 5, 7, 11, 13, 17, 31, 37, 71,
 * 73, 79, 97.
 *
 * How many circular primes are there below one million?
 */

unsigned int rotate(unsigned int x) {
  if (x < 10)
    return x;

  unsigned int digit = x % 10;

  x /= 10;
  x += std::pow(10, std::floor(std::log10(x)) + 1) * digit;

  return x;
}

std::vector<unsigned int> get_rotations(unsigned int x) {
  std::vector<unsigned int> ret_vec;

  unsigned int rotated = rotate(x);

  while (rotated != x) {
    ret_vec.push_back(rotated);
    rotated = rotate(rotated);
  }

  return ret_vec;
}

bool is_circular(unsigned int x) {
  // If any digits are 0, we say it's not circular
  unsigned int tmp = x;
  while (tmp != 0) {
    if (tmp % 10 == 0)
      return false;

    tmp /= 10;
  }

  auto rotations = get_rotations(x);
  for (auto r : rotations) {
    if (!is_prime(r))
      return false;
  }

  return true;
}

std::vector<unsigned int> compute_circular_primes_below(unsigned int upper) {
  auto all_primes = get_primes_up_to(upper);
  std::vector<unsigned int> circular_primes;

  // This could be made more efficient by keeping track of circular primes
  // already found, avoiding double-checking
  for (auto prime : all_primes) {
    if (is_circular(prime)) {
      log_info("Found circular prime:", prime);
      circular_primes.push_back(prime);
    }
  }

  return circular_primes;
}

int main(int argc, char** argv) {
  std::cout << compute_circular_primes_below(1000000).size() << std::endl;
}
