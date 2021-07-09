#include <iostream>
#include <cassert>
#include <cmath>

#include <cannon/log/registry.hpp>
#include <cannon/math/digits.hpp>
#include <cannon/math/palindromes.hpp>

using namespace cannon::log;
using namespace cannon::math;

/*!
 * The decimal number, 585 = 1001001001_2 (binary), is palindromic in both
 * bases.
 *
 * Find the sum of all numbers, less than one million, which are palindromic in
 * base 10 and base 2. 
 *
 * (Note that the palindromic number, in either base, may not include leading
 * zeros.)
 */

unsigned int compute_base_ten_and_two_palindrome_sum(unsigned int upper) {
  unsigned int sum = 0;

  auto palindromes = generate_palindromes(6);

  // Inefficient, since we can directly generate all base10 palindromes
  for (auto i : palindromes) {
    auto base_two = get_digits(i, 2);

    if (is_palindrome(base_two)) {
      log_info("Found palindrome:", i);
      sum += i;
    }
  }

  return sum;
}

int main(int argc, char** argv) {
  std::cout << compute_base_ten_and_two_palindrome_sum(1000000) << std::endl;
}
