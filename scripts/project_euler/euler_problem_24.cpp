#include <iostream>
#include <cassert>
#include <algorithm>

#include <cannon/log/registry.hpp>

using namespace cannon::log;

/*!
 * A permutation is an ordered arrangement of objects. For example, 3124 is one
 * possible permutation of the digits 1, 2, 3, and 4. If all of the permutations
 * are listed numerically or alphabetically, we call it lexicographic order. The
 * lexicographic permutations of 0, 1, and 2 are
 *
 *    012 021 102 120 201 210
 *
 * What is the millionth lexicographic permutation of the digits 0, 1, 2, 3, 4,
 * 5, 6, 7, 8, and 9?
 */

std::vector<unsigned int>
compute_lex_permutation(const std::vector<unsigned int> &digits,
                        unsigned int perm_num) {

  std::vector<unsigned int> sorted_digits(digits);
  std::sort(sorted_digits.begin(), sorted_digits.end());

  // No more suffixes to be permuted
  if (perm_num == 0)
    return sorted_digits;

  // There are (n - 1)! permutations of the suffix of the input digit vector
  unsigned int fact_num = 1;
  for (unsigned int i = 1; i < sorted_digits.size(); i++)
    fact_num *= i;

  assert(perm_num < (sorted_digits.size() * fact_num));

  unsigned int front_idx = perm_num / fact_num;
  unsigned int sub_perm_num = perm_num % fact_num;

  // The input permutation number divided by the number of suffix permutations
  // tells us which place should be in front
  auto front_digit = sorted_digits[front_idx];

  std::vector<unsigned int> sub_digits;
  for (auto& x : sorted_digits)
    if (x != front_digit)
      sub_digits.push_back(x);

  // Recurse to compute suffix permutation. This could be made more efficient
  // by swapping digits in an array rather than creating new vectors.
  auto sub_perm = compute_lex_permutation(sub_digits, sub_perm_num);

  std::vector<unsigned int> ret_vec;
  ret_vec.push_back(front_digit);
  ret_vec.insert(ret_vec.end(), sub_perm.begin(), sub_perm.end());
  return ret_vec;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "Please provide permutation number" << std::endl;
  }

  std::vector<unsigned int> digits;

  for (unsigned int i = 0; i < 10; ++i)
    digits.push_back(i);

  auto perm = compute_lex_permutation(digits, std::stoul(argv[1]));

  //std::cout << compute_lex_permutation(digits, 999999) << std::endl;
  for (auto& x : perm)
    std::cout << x;

  std::cout << std::endl;
}
