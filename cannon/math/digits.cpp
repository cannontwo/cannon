#include <cannon/math/digits.hpp>

#include <cmath>
#include <stdexcept>

std::vector<unsigned int> cannon::math::get_digits(unsigned int x,
                                                   unsigned int base) {
  std::vector<unsigned int> ret_vec;

  if (base < 2)
    throw std::runtime_error("Passed inappropriate basis");

  while(x != 0) {
    ret_vec.push_back(x % base);
    x /= base;
  }

  return ret_vec;
}

unsigned int cannon::math::from_digits(const std::vector<unsigned int> &digits,
                                       unsigned int base) {
  unsigned int ret = 0;
  for (unsigned int i = 0; i < digits.size(); ++i) {
    ret += std::pow(base, i) * digits[i];
  }

  return ret;
}
