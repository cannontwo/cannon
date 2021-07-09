#include <iostream>
#include <cassert>
#include <set>

#include <cannon/log/registry.hpp>

using namespace cannon::log;

/*!
 * We shall say that an n-digit number is pandigital if it makes use of all the
 * digits 1 to n exactly once; for example, the 5-digit number, 15234, is 1
 * through 5 pandigital.
 *
 * The product 7254 is unusual, as the identity, 39 * 186 = 7254, containing
 * multiplier, multiplicand, and product is 1 through 9 pandigital. 
 *
 * Find the sum of all products whose multiplicand/multiplier/product identity
 * can be written as a 1 through 9 pandigital. 
 *
 * HINT: Some products can be obtained in more than one way so be sure to only
 * include it once in your sum.
 */

bool check_digits(unsigned int x, std::vector<bool>& taken, unsigned int& num_digits) {
  while (x != 0) {
    unsigned int digit = x % 10;
    ++num_digits;

    // Zero is never part of a 1-9 pandigital
    if (digit == 0)
      return false;

    // Each digit must be taken exactly once
    if (taken[digit-1])
      return false;

    taken[digit-1] = true;
    x /= 10;
  }

  return true;
}

bool is_pandigital(unsigned int x, unsigned int y, unsigned int z) {
  std::vector<bool> taken(9, false);
  unsigned int num_digits = 0;

  if (!check_digits(x, taken, num_digits))
    return false;

  if (!check_digits(y, taken, num_digits))
    return false;

  if (!check_digits(z, taken, num_digits))
    return false;

  if (num_digits != 9)
    return false;

  // If we've gotten here, the combination must be a pandigital
  log_info("Found pandigital:", x, "*", y, "=", z);
  return true;
}

std::set<unsigned int> compute_pandigital_products() {
  std::set<unsigned int> ret_set;

  for (unsigned int i = 1; i < 100; ++i) {
    for (unsigned int j = 100; j < 10000; ++j) {
      if (is_pandigital(i, j, i*j))
        ret_set.insert(i * j);
    }
  }

  return ret_set;
}

int main(int argc, char** argv) {
  auto pandigitals = compute_pandigital_products();
  unsigned int sum = 0;

  for (auto x : pandigitals)
    sum += x;
  
  std::cout << sum << std::endl;
}
