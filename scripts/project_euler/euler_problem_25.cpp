#include <iostream>
#include <cassert>
#include <algorithm>

#include <cannon/log/registry.hpp>
#include <cannon/math/bignum.hpp>

using namespace cannon::log;
using namespace cannon::math;

/*!
 * The Fibonacci sequence is defined by the recurrence relation:
 * 
 *   F_n = F_{n-1} + F_{n-2}, where F_1 = 1 and F_2 = 1.
 *
 * Hence the first 12 terms will be:
 *   
 *   F_1 = 1
 *   F_2 = 1
 *   F_3 = 2
 *   F_4 = 3
 *   F_5 = 5
 *   F_6 = 8
 *   F_7 = 13
 *   F_8 = 21
 *   F_9 = 34
 *   F_10 = 55
 *   F_11 = 89
 *   F_12 = 144
 *
 * The 12th term, F_12, is the first term to contain three digits. 
 *
 * What is the index of the first term in the Fibonacci sequence to contain
 * 1000 digits?
 */

unsigned int compute_thousand_digit_fib() {
  BigUnsigned fib_1("1");
  BigUnsigned fib_2("1");
  unsigned int i = 2;

  while (fib_2.get_digits().size() < 1000) {
    ++i;
    BigUnsigned tmp = fib_1 + fib_2;

    fib_1 = fib_2;
    fib_2 = tmp;
  }

  return i;
}

int main(int argc, char** argv) {
  std::cout << compute_thousand_digit_fib() << std::endl;
}
