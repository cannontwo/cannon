#include <iostream>
#include <cassert>

#include <cannon/log/registry.hpp>

using namespace cannon::log;

/*!
 * In the United Kingdom the currency is made up of pound (L) and pence (p).
 * There are eight coins in general circulation:
 *
 *   1p, 2p, 5p, 10p, 20p, 50p, L1 (100p), and L2 (200p).
 *
 * It is possible to make L2 in the following way:
 *  
 *   1xL1 + 1x50p + 2x20p + 1x5p + 1x2p + 3x1p
 *
 * How many different ways can L2 be made using any number of coins?
 */

struct CoinCollection {
  unsigned int sum() const {
    return one_p + 2 * two_p + 5 * five_p + 10 * ten_p + 20 * twenty_p +
           50 * fifty_p + 100 * one_l + 200 * two_l;
  }

  unsigned int one_p = 0;
  unsigned int two_p = 0;
  unsigned int five_p = 0;
  unsigned int ten_p = 0;
  unsigned int twenty_p = 0;
  unsigned int fifty_p = 0;
  unsigned int one_l = 0;
  unsigned int two_l = 0;
};

std::ostream& operator<<(std::ostream& os, const CoinCollection& c) {
  os << c.two_l << "x2L + " 
     << c.one_l << "x1L + "
     << c.fifty_p << "x50p + "
     << c.twenty_p << "x20p + "
     << c.ten_p << "x10p + "
     << c.five_p << "x5p + "
     << c.two_p << "x2p + "
     << c.one_p << "x1p";

  return os;
}

std::vector<CoinCollection> compute_remaining_coin_combinations(
    unsigned int sum, const CoinCollection &so_far, unsigned int place) {

  std::vector<CoinCollection> ret_vec;
  unsigned int sum_diff = sum - so_far.sum();

  if (place >= 8 || so_far.sum() == sum) {
    ret_vec.push_back(so_far);
    return ret_vec;
  }

  if (so_far.sum() > sum)
    throw std::runtime_error("Sum so far exceeds desired sum");

  std::vector<CoinCollection> options;
  switch (place) {
    case 0:
      for (unsigned int i = 0; i <= sum_diff / 200; ++i) {
        CoinCollection tmp(so_far);
        tmp.two_l = i;
        options.push_back(tmp);
      }
      break;
    case 1:
      for (unsigned int i = 0; i <= sum_diff / 100; ++i) {
        CoinCollection tmp(so_far);
        tmp.one_l = i;
        options.push_back(tmp);
      }
      break;
    case 2:
      for (unsigned int i = 0; i <= sum_diff / 50; ++i) {
        CoinCollection tmp(so_far);
        tmp.fifty_p = i;
        options.push_back(tmp);
      }
      break;
    case 3:
      for (unsigned int i = 0; i <= sum_diff / 20; ++i) {
        CoinCollection tmp(so_far);
        tmp.twenty_p = i;
        options.push_back(tmp);
      }
      break;
    case 4:
      for (unsigned int i = 0; i <= sum_diff / 10; ++i) {
        CoinCollection tmp(so_far);
        tmp.ten_p = i;
        options.push_back(tmp);
      }
      break;
    case 5:
      for (unsigned int i = 0; i <= sum_diff / 5; ++i) {
        CoinCollection tmp(so_far);
        tmp.five_p = i;
        options.push_back(tmp);
      }
      break;
    case 6:
      for (unsigned int i = 0; i <= sum_diff / 2; ++i) {
        CoinCollection tmp(so_far);
        tmp.two_p = i;
        options.push_back(tmp);
      }
      break;
    case 7:
      CoinCollection tmp(so_far);
      tmp.one_p = sum - so_far.sum();
      options.push_back(tmp);
      break;
  }

  for (auto& option : options) {
    auto continuation = compute_remaining_coin_combinations(sum, option, place+1);
    ret_vec.insert(ret_vec.end(), continuation.begin(), continuation.end());
  }

  return ret_vec;
}

std::vector<CoinCollection> compute_coin_combinations(unsigned int sum) {
  CoinCollection start;
  auto combinations = compute_remaining_coin_combinations(sum, start, 0); 
  return combinations;
}

int main(int argc, char** argv) {
  std::cout << compute_coin_combinations(200).size() << std::endl;
}
