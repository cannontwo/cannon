#include <cannon/math/bignum.hpp>

#include <cassert>
#include <cmath>

using namespace cannon::math;

BigUnsigned::BigUnsigned(const std::string& num) {
  for (unsigned int i = 0; i < num.size(); i++) {
    char digit = num.at(i);
    assert(digit >= '0' && digit <= '9');

    digits_.push_back((unsigned int)digit - '0'); 
  }
}

BigUnsigned::BigUnsigned(const std::vector<unsigned int>& digits) :
  digits_(digits) {}

BigUnsigned::BigUnsigned(const BigUnsigned& o) : digits_(o.digits_) { }

BigUnsigned::BigUnsigned(BigUnsigned&& o) : digits_(std::move(o.digits_)) {
  o.digits_.clear();
}

BigUnsigned& BigUnsigned::operator=(const BigUnsigned& o) {
  digits_ = o.digits_;
  return *this;
}

BigUnsigned BigUnsigned::operator+(const BigUnsigned& o) const {
  std::vector<unsigned int> ret_digits;
  unsigned int upper = std::max(size(), o.size());
  unsigned int carry = 0;

  for (unsigned int i = 0; i < upper; i++) {
    unsigned int l = i < size() ? digits_[i] : 0;
    unsigned int r = i < o.size() ? o.digits_[i] : 0;
    unsigned int tmp = l + r + carry;

    if (tmp > 9) {
      ret_digits.push_back(tmp - 10);
      carry = 1;
    } else {
      ret_digits.push_back(tmp);
      carry = 0;
    }
  }

  if (carry != 0)
    ret_digits.push_back(carry);

  return BigUnsigned(ret_digits);
}

std::vector<unsigned int> BigUnsigned::get_digits() const {
  return digits_;
}

unsigned int BigUnsigned::size() const {
  return digits_.size();
}

// Free Functions
std::ostream& cannon::math::operator<<(std::ostream& os, const BigUnsigned& num) {
  std::vector<unsigned int> digits = num.get_digits();

  for (int i = num.size()-1; i >= 0; --i) {
    os << digits[i];
  }

  return os;
}

