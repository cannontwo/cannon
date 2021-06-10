#include <iostream>
#include <cmath>

/*!
 * A palindromic number reads the same both ways. The largest palindrome made
 * from the product of two 2-digit numbers is 9009 = 91 x 99. 
 *
 * Find the largest palindrome made from the product of two 3-digit numbers.
 */

bool is_palindrome(unsigned int x) {
  unsigned int reversed = 0;
  unsigned int tmp = x;

  unsigned int digits = std::floor(std::log10(x));

  for (unsigned int i = 0; i <= digits; i++) {
    unsigned int digit = tmp % 10;
    reversed += std::pow(10, digits - i) * digit;
    tmp /= 10;
  }

  return reversed == x;
}

unsigned int get_largest_palindrome_product() {
  unsigned int largest_palindrome = 0; 
  unsigned int prev_smaller = 100;

  for (unsigned int i = 999; i >= 100; --i) {

    // We only have to loop to the smaller number found previously, since the
    // outer loop number will have decreased
    for (unsigned int j = i; j >= prev_smaller; --j) {
      unsigned int tmp = i * j;

      if (is_palindrome(tmp) && tmp > largest_palindrome) {
        largest_palindrome = tmp;
        prev_smaller = j;
      }
    }

    // We can break as soon as the largest found palindrome is greater than the
    // square of the outer loop number, as there will be no larger found
    // palindrome.
    if (largest_palindrome != 0 && i * i < largest_palindrome)
      break;
  }

  return largest_palindrome;
}

int main() {
  std::cout << get_largest_palindrome_product() << std::endl;
}
