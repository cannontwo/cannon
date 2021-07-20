#include <iostream>
#include <cassert>
#include <cmath>

#include <cannon/log/registry.hpp>
#include <cannon/math/digits.hpp>

using namespace cannon::log;
using namespace cannon::math;

/*!
 * It can be seen that the number 125874 and its double 251748 contain exactly
 * the same digits, but in a different order.
 *
 * Find the smallest positive integer x such that 2x, 3x, 4x, 5x, and 6x contain
 * the same digits.
 */

bool is_permutation(unsigned int x, unsigned int y) {

  // Early out if different number of digits
  if (std::floor(std::log10(x)) != std::floor(std::log10(y)))
    return false;

  auto digits = get_digits(y);
  std::vector<bool> taken(digits.size(), false);
  
  while (x != 0) {
    unsigned int digit = x % 10;

    bool found = false;
    for (unsigned int i = 0; i < digits.size(); ++i) {
      if (!taken[i] && digits[i] == digit) {
        taken[i] = true;
        found = true;
        break;
      }
    }

    if (!found)
      return false;

    x /= 10;
  }

  return true;
}

unsigned int compute_smallest_multiple_permutation() {
  unsigned int i = 10;

  while (true) {

    log_info("Checking", i);
    if (is_permutation(i, 2*i) &&
        is_permutation(i, 3*i) &&
        is_permutation(i, 4*i) &&
        is_permutation(i, 5*i) &&
        is_permutation(i, 6*i))
      return i;
    
    ++i;

    // Anything over 2*10^n cannot satisfy the criterion, as 6*x will have more
    // digits
    if (i > 2 * std::pow(10, std::floor(std::log10(i))))
      i = std::pow(10, std::floor(std::log10(i)) + 1);
  }
}

int main(int argc, char **argv) {
  std::cout << compute_smallest_multiple_permutation() << std::endl;
}
