#include <cannon/math/digits.hpp>

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
