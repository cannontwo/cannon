#include <iostream>
#include <cassert>
#include <set>

#include <cannon/log/registry.hpp>
#include <cannon/math/bignum.hpp>

using namespace cannon::log;
using namespace cannon::math;

/*!
 * Consider all integer combinations of a^b for 2 <= a <= 5 and 2 <= b <= 5:
 *
 *   2^2 = 4, 2^3 = 8, 2^4 = 16, 2^5 = 32
 *   3^2 = 9, 3^3 = 28, 3^4 = 81, 3^5 = 243
 *   4^2 = 16, 4^3 = 64, 4^4 = 256, 4^5 = 1024
 *   5^2 = 25, 5^3 = 125, 5^4 = 625, 5^5 = 3125
 *
 * If they are then placed in numerical order, with any repeats removed, we get
 * the following sequence of 15 distinct terms:
 *   
 *   4, 8, 9, 16, 25, 27, 32, 64, 81, 125, 243, 256, 625, 1024, 3125
 *
 * How many distinct terms are in the sequence generated by a^b for 2 <= a <=
 * 100 and 2 <= b <= 100?
 */

std::set<BigUnsigned> compute_power_sequence(unsigned int a_lower,
                                             unsigned int a_upper,
                                             unsigned int b_lower,
                                             unsigned int b_upper) {

  std::set<BigUnsigned> ret_set;

  for (unsigned int a = a_lower; a <= a_upper; ++a) {
    BigUnsigned x(a);

    // Get to b_lower-1 powers of a
    for (unsigned int i = 2; i < b_lower; ++i) {
      x *= a;
    }

    for (unsigned int b = b_lower; b <= b_upper; ++b) {
      x *= a;
      ret_set.insert(x);
    }
  }

  return ret_set;
}

int main(int argc, char** argv) {
  std::cout << compute_power_sequence(2, 100, 2, 100).size() << std::endl;
}