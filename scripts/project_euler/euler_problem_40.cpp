#include <iostream>
#include <cassert>
#include <cmath>

#include <cannon/log/registry.hpp>

using namespace cannon::log;

/*!
 * An irrational decimal fraction is created by concatenating the positive integers:
 *
 *   0.123456789101112131415161718192021...
 *
 * It can be seen that the 12th digit of the fractional part is 1.
 *
 * If d_n represents the nth digit of the fractional part, find the value of
 * the following expression.
 *
 *   d_1 * d_10 * d_100 * d_1000 * d_10000 * d_100000 * d_1000000
 */

unsigned int compute_champernowne_digit(unsigned int d) {
  unsigned int digits = 0;
  unsigned int i = 1;

  while (digits < d) {
    digits += static_cast<unsigned int>(std::floor(std::log10(i)+1));
    ++i;
  }

  unsigned int diff = digits - d;

  unsigned int digit =
      ((i - 1) / static_cast<unsigned int>(std::pow(10, diff))) % 10;

  log_info("Digit", d, "is", digit);
  
  return digit;
}

unsigned int compute_champernowne_digit_product() {
  return compute_champernowne_digit(1) *
         compute_champernowne_digit(10) *
         compute_champernowne_digit(100) *
         compute_champernowne_digit(1000) *
         compute_champernowne_digit(10000) *
         compute_champernowne_digit(100000) *
         compute_champernowne_digit(1000000);
}

int main(int argc, char** argv) {
  std::cout << compute_champernowne_digit_product() << std::endl;
}
