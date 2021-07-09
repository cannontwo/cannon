#include <iostream>
#include <cassert>

#include <cannon/log/registry.hpp>

using namespace cannon::log;

/*!
 * Starting with number 1 and moving to the right in a clockwise direction, a 5
 * by 5 spiral is formed as follows:
 *
 *   21 22 23 24 25
 *   20  7  8  9 10
 *   19  6  1  2 11
 *   18  5  4  3 12
 *   17 16 15 14 13
 *
 * It can be verified that the sum of the numbers on the diagonals is 101. 
 *
 * What is the sum of the numbers on the diagonals in a 1001 by 1001 spiral
 * formed in the same way?
 */

unsigned long compute_spiral_diagonal_sum(unsigned int dim) {

  unsigned long sum = 1;
  unsigned long current = 1;
  for (unsigned int i = 1; 2*i+1 <= dim; ++i) {
    for (unsigned int j = 0; j < 4; ++j) {
      current += i * 2;
      log_info("Adding", current);
      sum += current;
    }
  }

  return sum;
}

int main(int argc, char** argv) {
  std::cout << compute_spiral_diagonal_sum(1001) << std::endl;
}
