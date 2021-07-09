#include <iostream>
#include <cassert>

#include <cannon/log/registry.hpp>
#include <cannon/math/nearly_equal.hpp>

using namespace cannon::log;
using namespace cannon::math;

/*!
 * The fraction 49/98 is a curious fraction, as an inexperienced mathematician
 * in attempting to simplify it may incorrectly believe that 49/98 = 4/8, which
 * is correct, is obtained by cancelling the 9s.
 *
 * We shall consider fractions like, 30/50 = 3/5, to be trivial examples.
 *
 * There are exactly four non-trivial examples of this type of fraction, less
 * than one in value, and containing two digits in the numerator and
 * denominator.
 *
 * If the product of these four fractions is given in its lowest common terms,
 * find the value of the denominator.
 */

bool is_curious(double simplified, unsigned int num, unsigned int denom) {
  // For this problem, we only consider fractions < 1
  if (num >= denom)
    return false;

  double frac = static_cast<double>(num) / static_cast<double>(denom);

  return nearly_equal(frac, simplified);
}

std::pair<unsigned int, unsigned int> get_curious_fraction_product() {
  unsigned int numerator_product = 1;
  unsigned int denominator_product = 1;

  for (unsigned int i = 1; i < 10; ++i) {

    for (unsigned int j = 1; j < 10; ++j) {
      for (unsigned int k = 1; k < 10; ++k) {
        double simplified_frac =
            static_cast<double>(j) / static_cast<double>(k);

        unsigned int num_1 = j*10 + i;
        unsigned int num_2 = i*10 + j;
        unsigned int denom_1 = k*10 + i;
        unsigned int denom_2 = i*10 + k;

        if (is_curious(simplified_frac, num_1, denom_1) || 
            is_curious(simplified_frac, num_1, denom_2) ||
            is_curious(simplified_frac, num_2, denom_1) ||
            is_curious(simplified_frac, num_2, denom_2)) {
          numerator_product *= j;
          denominator_product *= k;
          log_info("Found curious fraction");
        }
      }
    }
  }

  return std::make_pair(numerator_product, denominator_product);
}

int main(int argc, char** argv) {
  auto [num, denom] = get_curious_fraction_product();
  std::cout << num << "/" << denom << std::endl;
}
