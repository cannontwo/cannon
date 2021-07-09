#include <cannon/math/palindromes.hpp>

#include <cmath>

bool cannon::math::is_palindrome(const std::vector<unsigned int> &digits) {
  if (digits.size() == 1)
    return true;

  for (unsigned int i = 0; i < (digits.size() / 2) + 1; ++i) {
    if (digits[i] != digits[digits.size() - 1 - i])
      return false;
  }

  return true;
}

std::vector<unsigned int>
cannon::math::generate_palindromes_of_length(unsigned int length,
                                             unsigned int base, 
                                             bool outer) {
  std::vector<unsigned int> palindromes;

  unsigned int lower = outer ? 1 : 0;

  if (length == 2) {
    for (unsigned int i = lower; i < base; ++i) {
      unsigned int pal = base * i + i;
      palindromes.push_back(pal);
    }
  } else if (length == 1) {
    for (unsigned int i = lower; i < base; ++i) {
      palindromes.push_back(i);
    }
  } else {
    for (unsigned int i = lower; i < base; ++i) {
      auto inner_pals = generate_palindromes_of_length(length - 2, base, false);
      for (auto p : inner_pals) {
        unsigned new_pal = i;
        new_pal += p * base;
        new_pal += std::pow(base, length - 1) * i;
        palindromes.push_back(new_pal);
      }
    }
  }

  return palindromes;
}

std::vector<unsigned int>
cannon::math::generate_palindromes(unsigned int max_length, unsigned int base) {
  std::vector<unsigned int> ret_vec;
  for (unsigned int i = 1; i <= max_length; ++i) {
    auto pals = generate_palindromes_of_length(i, base);
    ret_vec.insert(ret_vec.end(), pals.begin(), pals.end());
  }

  return ret_vec;
}
