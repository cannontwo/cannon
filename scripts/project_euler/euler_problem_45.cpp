#include <iostream>
#include <cassert>
#include <cmath>

#include <cannon/log/registry.hpp>

using namespace cannon::log;

/*!
 * Triangle, pentagonal, and hexagonal numbers are generated by the following formulae:
 *
 *   Triangle   T_n = n(n+1)/2  1, 3, 6, 10, 15, ...
 *   Pentagonal P_n = n(3n-1)/2 1, 5, 12, 22, 35, ...
 *   Hexagonal  H_n = n(2n-1)   1, 6, 15, 28, 45, ...
 *
 * It can be verified that T_285 = P_165 = H_143 = 40755.
 *
 * Find the next triangle number that is also pentagonal and hexagonal.
 */

bool is_pentagonal(unsigned long x) {
  // A number x is pentagonal if 3n^2 - n - 2x has a positive integer zero in n divisible by 6. 
  
  double sqrt_part = std::sqrt(1 + 24 * x);
  if (std::floor(sqrt_part) == sqrt_part) {
    return ((1 + static_cast<unsigned long>(sqrt_part)) % 6) == 0;
  } else
    return false;
}

bool is_hexagonal(unsigned long x) {
  // A number x is hexagonal if 2n^2 - n - x has a positive integer zero in n divisible by 4.
  
  double sqrt_part = std::sqrt(1 + 8 * x);
  if (std::floor(sqrt_part) == sqrt_part) {
    return ((1 + static_cast<unsigned long>(sqrt_part)) % 4) == 0;
  } else
    return false;
}

unsigned int compute_next_tri_pent_hex_num() {
  unsigned int n = 286;
  while (true) {
    unsigned long t_n = n * (n + 1) / 2;

    if (is_hexagonal(t_n) && is_pentagonal(t_n))
      return t_n;
    else
      ++n;
  }
}

int main(int argc, char **argv) {
  std::cout << compute_next_tri_pent_hex_num() << std::endl;
}
