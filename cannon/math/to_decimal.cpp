#include <cannon/math/to_decimal.hpp>

#include <sstream>
#include <cassert>

#include <cannon/log/registry.hpp>

using namespace cannon::log;

std::pair<std::vector<unsigned int>, std::vector<unsigned int>>
cannon::math::compute_decimal_reciprocal(unsigned int x) {
  assert(x > 1);

  std::vector<unsigned int> digits;
  std::vector<unsigned int> remainders;
  unsigned int remainder = 10;

  while (remainder != 0) {
    unsigned int new_digit = remainder / x;
    unsigned int new_remainder = (remainder % x) * 10;

    digits.push_back(new_digit);
    remainders.push_back(remainder);

    // A cycle occurs when we have the same remainder in a new position
    for (unsigned int i = 0; i < remainders.size(); ++i) {
      if (new_remainder == remainders[i]) {
        // We have a cycle from i to the end of digits
        
        std::vector<unsigned int> non_repeating, repeating;
        for (unsigned int j = 0; j < digits.size(); ++j) {
          if (j < i)
            non_repeating.push_back(digits[j]);
          else
            repeating.push_back(digits[j]);
        }

        return std::make_pair(non_repeating, repeating);
      }
    }

    remainder = new_remainder;
  }

  // If we got here, there is no cycle
  std::vector<unsigned int> empty;
  return std::make_pair(digits, empty);

}

void cannon::math::print_decimal_reciprocal(unsigned int x) {
  auto [non_repeating, repeating] = compute_decimal_reciprocal(x);

  // Print number
  std::stringstream ss;
  ss << "0.";
  for (auto& x : non_repeating)
    ss << x;

  if (repeating.size() > 0) {
    ss << "(";
    for (auto& x : repeating)
      ss << x;
    ss << ")";
  }

  log_info("1 /", x, "=", ss.str());
}
