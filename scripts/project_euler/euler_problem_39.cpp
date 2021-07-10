#include <iostream>
#include <cassert>
#include <cmath>

#include <cannon/log/registry.hpp>

using namespace cannon::log;

/*!
 * If p is the perimeter of a right angle triangle with integral side lengths,
 * {a, b, c}, there are exactly three solutions for p = 120:
 *
 *   {20, 48, 52}, {24, 45, 51}, {30, 40, 50}
 *
 * For which value of p <= 1000, is the number of solutions maximized?
 */

unsigned int compute_max_integer_right_triangles(unsigned int upper) {
  unsigned int max_perim = 0;
  unsigned int max_triangles = 0;

  for (unsigned int i = 3; i <= upper; ++i) {
    unsigned int combs = 0;

    // Derived from Pythagoras's Theorem
    for (unsigned int j = 1; j <= (i*i - 2*i) / (2*i - 2); ++j) {
      if (i*(i - 2*j) % (2*i - 2*j) == 0)
        ++combs;
    }

    combs /= 2;
    if (combs != 0)
      log_info("Got", combs, "integral right triangles for perimeter", i);

    if (combs > max_triangles) {
      max_perim = i;
      max_triangles = combs;
    }
  }

  return max_perim;
}

int main(int argc, char** argv) {
  std::cout << compute_max_integer_right_triangles(1000) << std::endl;
}
