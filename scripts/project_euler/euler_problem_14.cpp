#include <iostream>
#include <cmath>

#include <cannon/log/registry.hpp>

using namespace cannon::log;

/*!
 * The following iterative sequence is defined for the set of positive integers:
 *
 *     n -> n / 2 (if n is even)
 *     n -> 3n + 1 (if n is odd)
 *
 * Using the rule above and starting with 13, we generate the following sequence:
 *
 *     13 -> 40 -> 20 -> 10 -> 5 -> 16 -> 8 -> 4 -> 2 -> 1
 *
 * It can be seen that this sequence (starting at 13 and finishing at 1)
 * contains 10 terms. Although it has not been proved yet (Collatz Conjecture),
 * it is thought that all starting numbers finish at 1.
 *
 * Which starting number, under one million, produces the longest chain?
 *
 * Note: Once the chain starts the terms are allowed to go above one million.
 */

unsigned int get_collatz_sequence_length(unsigned long start) {
  unsigned long current = start;
  unsigned int seq_length = 1;

  while (current != 1) {
    if (current % 2 == 0)
      current /= 2;
    else {
      current = 3 * current + 1;
    }

    seq_length += 1;
  }
  
  return seq_length;
}

unsigned int get_longest_collatz_sequence() {
  unsigned int longest_collatz_num = 0;
  unsigned int longest_collatz_length = 0;

  for (unsigned int i = 1; i < 1e6; ++i) {
    unsigned int collatz_length = get_collatz_sequence_length(i);

    if (collatz_length > longest_collatz_length) {
      longest_collatz_length = collatz_length;
      longest_collatz_num = i;
    }
  }

  return longest_collatz_num;
}

int main(int argc, char** argv) {
  std::cout << get_longest_collatz_sequence() << std::endl;
}
