#include <iostream>
#include <cassert>
#include <cmath>

#include <cannon/log/registry.hpp>
#include <cannon/math/digits.hpp>

using namespace cannon::log;
using namespace cannon::math;

/*!
 * Take the number 192 and multiply it by each of 1, 2, and 3:
 *
 *   192 * 1 = 192
 *   192 * 2 = 384
 *   192 * 3 = 576
 *
 * By concatenating each product we get the 1 to 9 pandigital, 192384576. We
 * will call 192384576 the concatenated product of 192 and (1, 2, 3). 
 *
 * The same can be achieved by starting with 9 and multiplying by 1, 2, 3, 4,
 * and 5, giving the pandigital 918273645, which is the concatenated product of
 * 9 and (1, 2, 3, 4, 5).
 *
 * What is the largest 1 to 9 pandigital 9-digit number that can be formed as
 * the concatenated product of an integer with (1, 2, ..., n) where n > 1?
 */

unsigned long compute_concatenated_product(unsigned int x, unsigned int n) {
  unsigned long concat = 0; 

  for (unsigned int i = 1; i <= n; ++i) {
    unsigned int new_term = x * i;
    concat *= std::pow(10, std::floor(std::log10(new_term)) + 1);
    concat += new_term;
  }

  return concat;
}

bool is_pandigital(unsigned long x) {
  std::vector<bool> taken(9, false);

  if (x >= std::pow(10, 9) || x < std::pow(10, 8))
    return false;

  auto digits = get_digits(x);

  for (unsigned int i = 0; i < digits.size(); ++i) {
    if (digits[i] == 0)
      return false;

    // Each digit must be taken exactly once
    if (taken[digits[i]-1])
      return false;

    taken[digits[i]-1] = true;
  }

  return true;
}

unsigned int compute_largest_concatenated_pandigital() {
  unsigned int largest = 0;

  for (unsigned int n = 2; n <= 9; ++n) {
    for (unsigned int i = 1; i < std::pow(10, 10 / n); ++i) {
      //log_info("Checking", i, "* (1, ...,", n, ")");
      auto candidate = compute_concatenated_product(i, n);
      
      if (is_pandigital(candidate)) {
        log_info("Found pandigital concatenated product:", candidate, "for", i, "* (1, ...,", n, ")");
        if (candidate > largest) {
          largest = candidate;
        }
      }
    }
  }

  return largest;
}

int main(int argc, char** argv) {
  std::cout << compute_largest_concatenated_pandigital() << std::endl;
}
