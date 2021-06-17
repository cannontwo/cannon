#include <iostream>

#include <cannon/log/registry.hpp>

using namespace cannon::log;

/*!
 * If the numbers 1 to 5 are written out in words: one, two, three, four, five,
 * then there are 3 + 3 + 5 + 4 + 4 = 19 letters used in total.
 *
 * If all the numbers from 1 to 1000 (one thousand) inclusive were written out
 * in words, how many letters would be used?
 *
 * Note: Do not count spaces or hyphens. For example, 342 (three hundred and
 * forty-two) contains 23 letters and 115 (one hundred and fifteen) contains 20
 * letters. 
 */

unsigned int get_number_letters(unsigned int x) {
  if (x == 0)
    return 0;

  if (1 <= x && x < 20) {
    switch (x) {
      case 1:
        return 3;
      case 2:
        return 3;
      case 3:
        return 5;
      case 4:
        return 4;
      case 5:
        return 4;
      case 6:
        return 3;
      case 7:
        return 5;
      case 8:
        return 5;
      case 9:
        return 4;
      case 10:
        return 3;
      case 11:
        return 6;
      case 12:
        return 6;
      case 13:
        return 8;
      case 14:
        return 8;
      case 15:
        return 7;
      case 16:
        return 7;
      case 17:
        return 9;
      case 18:
        return 8;
      case 19:
        return 8;
    }
  } else if (20 <= x && x < 100) {
    unsigned int tens = x / 10;
    unsigned int ones = x - (10 * tens);

    unsigned int tens_letters = 0;

    switch (tens) {
      case 2:
        tens_letters = 6;
        break;
      case 3:
        tens_letters = 6;
        break;
      case 4:
        tens_letters = 5;
        break;
      case 5:
        tens_letters = 5;
        break;
      case 6:
        tens_letters = 5;
        break;
      case 7:
        tens_letters = 7;
        break;
      case 8:
        tens_letters = 6;
        break;
      case 9:
        tens_letters = 6;
        break;
    }

    return get_number_letters(ones) + tens_letters;
  } else if (x < 1000) {
    unsigned int hundreds = x / 100;
    unsigned int hundreds_letters = get_number_letters(hundreds);
    unsigned int tens = x - (hundreds * 100);

    if (tens == 0)
      return hundreds_letters + 7;

    return hundreds_letters + get_number_letters(tens) + 10;
  } else {
    return 11;
  }
}

unsigned int get_number_letters_under_thousand() {
  unsigned int sum = 0;
  for (unsigned int i = 1; i <= 1000; i++) {
    sum += get_number_letters(i);
  }

  return sum;
}

int main(int argc, char** argv) {
  std::cout << get_number_letters_under_thousand() << std::endl;
  std::cout << get_number_letters(std::stoul(argv[1])) << std::endl;
}
