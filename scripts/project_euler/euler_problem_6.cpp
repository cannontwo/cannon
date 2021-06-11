#include <iostream>
#include <cmath>

/*!
 * The sum of the squares of the first ten natural numbers is
 *
 *    1^2 + 2^2 + ... + 10^2 = 385
 *
 * The square of the sum of the first ten natural numbers is
 * 
 *    (1 + 2 + ... + 10)^2 = 55^2 = 3025
 *
 * Hence the difference between the sum of the squares of the first ten natural
 * numbers and the square of the sum is 3025 - 385 - 2640. 
 *
 * Find the difference between the sum of the squares of the first one hundred
 * natural numbers and the square of the sum.
 */

unsigned int find_square_sum_difference(unsigned int upper) {
  unsigned int squared_sum = std::pow((upper * (upper + 1) / 2), 2);

  unsigned int sum_of_squares = upper * (upper + 1) * (2*upper + 1) / 6;
  //for (unsigned int i = 1; i <= upper; i++) {
  //  sum_of_squares += std::pow(i, 2);
  //}

  return squared_sum - sum_of_squares;
}

int main() {
  std::cout << find_square_sum_difference(100) << std::endl;
}
