#include <iostream>
#include <cassert>
#include <cmath>

#include <cannon/log/registry.hpp>
#include <cannon/math/primes.hpp>
#include <cannon/math/digits.hpp>

using namespace cannon::log;
using namespace cannon::math;

/*!
 * By replacing the 1st digit of the 2-digit number *3, it turns out that six
 * of the nine possible values: 13, 23, 43, 53, 73, 83, are all prime.
 *
 * By replacing the 3rd and 4th digits of 56**3 with the same digit, this
 * 5-digit number is the first example having seven primes among the ten
 * generated numbers, yielding the family: 56003, 56113, 56333, 56443, 56663,
 * 56773, and 56993. Consequently 56003, being the first member of this family,
 * is the smallest prime with this property.
 *
 * Find the smallest prime which, by replacing part of the number (not
 * necessarily adjacent digits) with the same digit, is part of an eight prime
 * value family.
 */

void choose_replacements(const std::vector<unsigned int> &places,
                         unsigned int length, unsigned int start,
                         std::vector<unsigned int> comb,
                         std::vector<std::vector<unsigned int>> &all_combs) {
  if (length == 0) {
    all_combs.push_back(comb);
    return;
  }

  for (unsigned int i = start; i <= places.size() - length; ++i) {
    comb[comb.size() - length] = places[i];
    choose_replacements(places, length-1, i+1, comb, all_combs);
  }
}

std::vector<unsigned int>
compute_largest_i_digit_family(const std::vector<unsigned int> &digits,
                               unsigned int num_reps) {
  std::vector<std::vector<unsigned int>> all_reps;

  std::vector<unsigned int> places(digits.size());
  for (unsigned int i = 0; i < digits.size(); ++i)
    places[i] = i;

  std::vector<unsigned int> initial(num_reps);

  choose_replacements(places, num_reps, 0, initial, all_reps);

  std::vector<unsigned int> largest_family;

  for (auto& rep : all_reps) {
    std::vector<unsigned int> rep_digits(digits);
    std::vector<unsigned int> prime_fam;

    for (unsigned int i = 0; i < 10; ++i) {
      for (unsigned int j = 0; j < rep.size(); ++j) {
        rep_digits[rep[j]] = i;
      }

      if (rep_digits[rep_digits.size() - 1] == 0)
        continue;

      unsigned int rep_num = from_digits(rep_digits);
      if (is_prime(rep_num))
        prime_fam.push_back(rep_num);
    }

    if (prime_fam.size() > largest_family.size())
      largest_family = prime_fam;
  }

  return largest_family;
}

std::vector<unsigned int> compute_largest_replacement_primes(unsigned int p) {
  auto digits = get_digits(p);
  std::vector<unsigned int> largest_family;

  for (unsigned int i = 1; i < digits.size(); ++i) {
    // Replace i digits 
    auto i_family = compute_largest_i_digit_family(digits, i);
    if (i_family.size() > largest_family.size())
      largest_family = i_family;
  }

  return largest_family;
}

unsigned int compute_prime_digit_replacement_family(unsigned int upper, unsigned int fam_size) {
  auto primes = get_primes_up_to(upper);

  for (auto &p : primes) {
    if (p < 10)
      continue;

    auto replacement_primes = compute_largest_replacement_primes(p);
    log_info("Prime", p, "has", replacement_primes.size(), "digit replacement primes:");
    for (auto& x : replacement_primes)
      log_info("\t", x);

    if (replacement_primes.size() >= fam_size)
      return replacement_primes[0];
  }

  return 0;
}

int main(int argc, char **argv) {
  std::cout << compute_prime_digit_replacement_family(1000000, 8) << std::endl;
}
