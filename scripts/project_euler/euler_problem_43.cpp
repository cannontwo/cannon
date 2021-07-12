#include <iostream>
#include <cassert>
#include <cmath>

#include <cannon/log/registry.hpp>

using namespace cannon::log;

/*!
 * The number, 1406357289, is a 0 to 9 pandigital number because it is maade up
 * of each of the digits 0 to 9 in some order, but it also has a rather
 * interesting sub-string divisibility property.
 *
 * Let d_1 be the 1st digit, d_2 be the 2nd digit, and so on. In this way, we
 * note the following:
 *
 *   d_2 d_3 d_4 = 406 is divisible by 2
 *   d_3 d_4 d_5 = 063 is divisible by 3
 *   d_4 d_5 d_6 = 635 is divisible by 5
 *   d_5 d_6 d_7 = 357 is divisible by 7
 *   d_6 d_7 d_8 = 572 is divisible by 11
 *   d_7 d_8 d_9 = 728 is divisible by 13
 *   d_8 d_9 d_10 = 289 is divisible by 17
 *
 * Find the sum of all 0 to 9 pandigital numbers with this property.
 */

std::vector<unsigned long> generate_curious_pandigitals() {
  std::vector<unsigned long> ret_vec;

  for (unsigned long d1 = 0; d1 <= 9; ++d1) {
    for (unsigned long d2 = 0; d2 <= 9; ++d2) {
      if (d1 == d2)
        continue;

      for (unsigned long d3 = 0; d3 <= 9; ++d3) {
        if (d3 == d1 || d3 == d2)
          continue;

        for (unsigned long d4 = 0; d4 <= 9; d4 += 2) {
          if (d4 == d1 || d4 == d2 || d4 == d3)
            continue;

          for (unsigned long d5 = 0; d5 <= 9; ++d5) {
            if (d5 == d1 || d5 == d2 || d5 == d3 || d5 == d4)
              continue;

            if ((d3 + d4 + d5) % 3 != 0)
              continue;

            for (unsigned long d6 = 0; d6 <= 9; d6 += 5) {
              if (d6 == d1 || d6 == d2 || d6 == d3 || d6 == d4 || d6 == d5)
                continue;

              for (unsigned long d7 = 0; d7 <= 9; ++d7) {
                if (d7 == d1 || d7 == d2 || d7 == d3 || d7 == d4 || d7 == d5 ||
                    d7 == d6)
                  continue;

                if ((100 * d5 + 10 * d6 + d7) % 7 != 0)
                  continue;

                for (unsigned long d8 = 0; d8 <= 9; ++d8) {
                  if (d8 == d1 || d8 == d2 || d8 == d3 || d8 == d4 ||
                      d8 == d5 || d8 == d6 || d8 == d7)
                    continue;

                  if ((100 * d6 + 10 * d7 + d8) % 11 != 0)
                    continue;

                  for (unsigned long d9 = 0; d9 <= 9; ++d9) {
                    if (d9 == d1 || d9 == d2 || d9 == d3 || d9 == d4 ||
                        d9 == d5 || d9 == d6 || d9 == d7 || d9 == d8)
                      continue;

                    if ((100 * d7 + 10 * d8 + d9) % 13 != 0)
                      continue;

                    for (unsigned long d10 = 0; d10 <= 9; ++d10) {
                      if (d10 == d1 || d10 == d2 || d10 == d3 || d10 == d4 ||
                          d10 == d5 || d10 == d6 || d10 == d7 || d10 == d8 ||
                          d10 == d9)
                        continue;

                      if ((100 * d8 + 10 * d9 + d10) % 17 != 0)
                        continue;

                      unsigned long pan = std::pow(10, 0) * d10;
                      pan += std::pow(10, 1) * d9;
                      pan += std::pow(10, 2) * d8;
                      pan += std::pow(10, 3) * d7;
                      pan += std::pow(10, 4) * d6;
                      pan += std::pow(10, 5) * d5;
                      pan += std::pow(10, 6) * d4;
                      pan += std::pow(10, 7) * d3;
                      pan += std::pow(10, 8) * d2;
                      pan += std::pow(10, 9) * d1;

                      ret_vec.push_back(pan);
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  return ret_vec;
}

unsigned int sub_string_num(unsigned long x, unsigned int min,
                            unsigned int max) {
  unsigned long old_x = x;
  for (int i = 10 - max; i > 0; --i) {
    x /= 10;
  }
  log_info("x is now", x);

  unsigned int sub_num =
      x % static_cast<unsigned int>(std::pow(10, max - min + 1));

  log_info("Digits", min, "to", max, "of", old_x, "are", sub_num);

  return sub_num;
}

unsigned long compute_curious_pandigital_sum() {
  auto pandigitals = generate_curious_pandigitals();

  unsigned long sum = 0;
  for (auto x : pandigitals) {
    if (x < std::pow(10, 9))
      continue;

    if (sub_string_num(x, 2, 4) % 2 == 0 &&
        sub_string_num(x, 3, 5) % 3 == 0 &&
        sub_string_num(x, 4, 6) % 5 == 0 &&
        sub_string_num(x, 5, 7) % 7 == 0 &&
        sub_string_num(x, 6, 8) % 11 == 0 &&
        sub_string_num(x, 7, 9) % 13 == 0 &&
        sub_string_num(x, 8, 10) % 17 == 0)
      sum += x;
  }

  return sum;
}

int main(int argc, char **argv) {
  std::cout << compute_curious_pandigital_sum() << std::endl;
}
