#include <iostream>
#include <cassert>
#include <cmath>

#include <Eigen/Dense>

#include <cannon/log/registry.hpp>

using namespace Eigen;

using namespace cannon::log;

/*!
 * There are exactly ten ways of selecting three from five, 12345:
 *
 *   123, 124, 125, 134, 135, 145, 234, 245, 345
 *
 * In combinatorics, we use the notation, 5c3 = 10.
 *
 * In generall, ncr = n! / r!*(n - r)!, where r <= n.
 *
 * It is not until n = 23 that a value exceeds one-million: 23c10 = 1144066.
 *
 * How many, not necessarily distinct, values of ncr for 1 <= n <= 100, are
 * greater than one-million.
 */

unsigned int compute_num_combinations_over_million() {
  unsigned int num_greater = 0;
  MatrixXd pascal = MatrixXd::Zero(101, 101);
  pascal.col(0) = VectorXd::Ones(101);

  for (unsigned int n = 1; n <= 100; ++n) {
    for (unsigned int r = 1; r <= n; ++r) {
      pascal(n, r) = pascal(n - 1, r) + pascal(n - 1, r -1);

      if (pascal(n, r) > 1000000) {
        pascal(n, r) = 1000000;
        ++num_greater;
      }
    }
  }

  return num_greater;
}

int main(int argc, char **argv) {
  std::cout << compute_num_combinations_over_million() << std::endl;
}
