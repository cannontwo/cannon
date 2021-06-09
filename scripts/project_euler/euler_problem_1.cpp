#include <iostream>

#include <cannon/log/registry.cpp>

/*!
 * \file
 * \brief If we list all the natural numbers below 10 that are multiples of 3 or 5, we
 * get 3, 5, 6, 9. The sum of these multiples is 23.
 *
 * Find the sum of all the multiples of 3 or 5 below 1000. 
 */

unsigned int get_three_five_sum_naive(unsigned int upper) {
  unsigned int sum = 0;
  unsigned int i = 0;

  while (3*i < upper) {

    sum += 3 * i;
    if (i % 3 != 0 && 5*i < upper) 
      sum += 5*i;

    ++i;
  }

  return sum;
}

unsigned int get_three_five_sum_fast(unsigned int upper) {
  unsigned int num_3s = upper / 3;
  unsigned int num_5s = upper / 5;
  unsigned int num_15s = upper / 15;

  // We only want multiples strictly less than the upper limit
  if (3*num_3s == upper)
    num_3s -= 1;

  if (5*num_5s == upper)
    num_5s -= 1;

  // Closed form expression for sum of 1, ..., n gives better scalability
  unsigned int three_sum = (num_3s * (num_3s + 1) / 2) * 3;
  unsigned int five_sum = (num_5s * (num_5s + 1) / 2) * 5;

  // We have to subtract numbers which get counted twice
  unsigned int fifteen_sum = (num_15s * (num_15s + 1) / 2) * 15;

  return three_sum + five_sum - fifteen_sum;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    log_error("This script takes one argument, which is the upper limit below "
              "which to find all multiples of 3 or 5.");
    return 1;
  }

  //std::cout << get_three_five_sum_naive(std::stoi(argv[1])) << std::endl;
  std::cout << get_three_five_sum_fast(std::stoi(argv[1])) << std::endl;
}
