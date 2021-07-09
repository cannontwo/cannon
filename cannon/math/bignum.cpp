#include <cannon/math/bignum.hpp>

#include <cassert>
#include <cmath>
#include <algorithm>

using namespace cannon::math;

BigUnsigned::BigUnsigned(unsigned int x) {
  if (x == 0) {
    digits_.push_back(0);
    return;
  }

  while (x != 0) {
    digits_.push_back(x % 10);
    x /= 10;
  }
}

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
  BigUnsigned tmp(*this);
  tmp += o;
  return tmp;
}

BigUnsigned BigUnsigned::operator*(unsigned int o) const {
  BigUnsigned tmp(*this);
  tmp *= o;
  return tmp;
}

BigUnsigned& BigUnsigned::operator+=(const BigUnsigned& o) {
  std::vector<unsigned int> new_digits;
  unsigned int upper = std::max(size(), o.size());
  unsigned int carry = 0;

  for (unsigned int i = 0; i < upper; i++) {
    unsigned int l = i < size() ? digits_[i] : 0;
    unsigned int r = i < o.size() ? o.digits_[i] : 0;
    unsigned int tmp = l + r + carry;

    if (tmp > 9) {
      new_digits.push_back(tmp - 10);
      carry = 1;
    } else {
      new_digits.push_back(tmp);
      carry = 0;
    }
  }

  if (carry != 0)
    new_digits.push_back(carry);

  digits_ = new_digits;
  return *this;
}

BigUnsigned& BigUnsigned::operator*=(const BigUnsigned& o) {
  std::vector<unsigned int> new_digits;
  unsigned int carry = 0;

  for (unsigned int i = 0; i <= size() + o.size() - 2; ++i) {
    unsigned int digit_sum = carry;

    for (unsigned int j = 0; j <= std::min(i, size()-1); ++j) {
      if (i - j < o.size())
        digit_sum += digits_[j] * o.digits_[i - j];
    }

    new_digits.push_back(digit_sum % 10);
    carry = digit_sum / 10;
  }

  while (carry != 0) {
    new_digits.push_back(carry % 10);
    carry /= 10;
  }

  digits_ = new_digits;

  return *this;
}

bool BigUnsigned::operator<(const BigUnsigned& o) const {
  if (size() != o.size())
    return size() < o.size();
  else {
    for (int i = size()-1; i >=0; --i) {
      if (digits_[i] == o.digits_[i])
        continue;
      else
        return digits_[i] < o.digits_[i];
    }

    return false;
  }
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

