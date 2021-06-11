#include <iostream>
#include <cassert>
#include <cmath>

/*!
 * A Pythagorean triplet is a set of three natural numbers, a < b < c, for which
 *
 *     a^2 + b^2 = c^2
 *
 * For example 3^2 + 4^2 = 9 + 16 = 25 = 5^2.
 *
 * There exists exactly one Pythagorean triplet for which a + b + c = 1000.
 * Find the product abc.
 */

unsigned long find_special_pythagorean_triplet_product(unsigned int sum) {
  
  for (unsigned int a = 1; a <= sum - 2; a++) {
    for (unsigned int b = 1; b <= sum - a - 1; b++) {
      unsigned int c = sum - a - b; 

      assert(c >= 1);
      assert(a + b + c = sum);

      if (std::pow(a, 2) + std::pow(b, 2) == std::pow(c, 2)) {
        return a * b * c;
      }
    }
  }

  return 0;

}

int main() {
  std::cout << find_special_pythagorean_triplet_product(1000) << std::endl;
}
