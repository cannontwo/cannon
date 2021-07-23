#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <sstream>

#include <cannon/log/registry.hpp>


using namespace cannon::log;

/*!
 * In the card game poker, a hand consists of five cards and are ranked, from
 * lowest to highest, in the following way:
 *
 *   High Card: Highest value card
 *   One Pair: Two cards of the same value
 *   Two Pairs: Two different pairs
 *   Three of a Kind: Three cards of the same value
 *   Straight: All cards are consecutive values
 *   Flush: All cards of the same suit
 *   Full House: Three of a kind and a pair
 *   Four of a Kind: Four cards of the same value
 *   Straight Flush: All cards are consecutive values of the same suit
 *   Royal Flush: Ten, Jack, Queen, King, Ace, in same suit
 *
 * The cards are valued in the order:
 *   
 *   2, 3, 4, 5, 6, 7, 8, 9, 10, Jack, Queen, King, Ace
 *
 * If two players have the same ranked hands then the rank made up of the
 * highest value wins; for example, a pair of eights beats a pair of fives. But
 * if two ranks tie, for example, both players have a pair of queens, then the
 * highest cards in each hand are compared. If the highest cards tie then the
 * next highest cards are compared, and so on.
 *
 * The file poker.txt contains one-thousand random hands dealt to two players.
 * Each line of the file contains ten cards (separated by a single space): the
 * first five are Player 1's cards and the last five are Player 2's cards. You
 * can assume that all hands are valid (no invalid characters or repeated
 * cards), each player's hand is in no specific order, and in each hand there
 * is a clear winner.
 *
 * How many hands does Player 1 win?
 */

class Card {
  public:
    Card(char value, char suit) : value_(value), suit_(suit) {}

    friend bool operator<(const Card& a, const Card& b) {
      if (a.value_ == 'A') {
        return false;
      } else if (a.value_ == 'K') {
        return b.value_ == 'A';
      } else if (a.value_ == 'Q') {
        return b.value_ == 'K' || b.value_ == 'A';
      } else if (a.value_ == 'J') {
        return b.value_ == 'Q' || b.value_ == 'K' || b.value_ == 'A';
      } else if (a.value_ == 'T') {
        return b.value_ == 'J' || b.value_ == 'Q' || b.value_ == 'K' ||
               b.value_ == 'A';
      } else {
        if (b.value_ >= 'A' && b.value_ <= 'Z')
          return true;
        else
          return a.value_ < b.value_;
      }
    }

    char value() const {
      return value_;
    }

    char suit() const {
      return suit_;
    }

    char next_value() const {
      if (value_ >= '2' && value_ <= '8')
        return value_ + 1;
      else if (value_ == '9')
        return 'T';
      else if (value_ == 'T')
        return 'J';
      else if (value_ == 'J')
        return 'Q'; 
      else if (value_ == 'Q')
        return 'K';
      else if (value_ == 'K')
        return 'A';
    }

    friend std::ostream& operator<<(std::ostream& os, const Card& c) {
      os << c.value_ << c.suit_;
      return os;
    }

  private:
    char value_;
    char suit_;
};

class Hand {
  public:
    Hand(const std::vector<Card> cards) : cards_(cards) {
      assert(cards_.size() == 5); 
      std::sort(cards_.begin(), cards_.end());
    }

    bool is_royal_flush() const {
      return is_straight_flush() && cards_[0].value() == 'T';
    }

    bool is_straight_flush() const {
      return is_straight() && is_flush();
    }

    bool is_four_of_a_kind() const {
      for (unsigned int i = 0; i < cards_.size() - 3; ++i) {
        if (cards_[i].value() == cards_[i+1].value() &&
            cards_[i].value() == cards_[i+2].value() &&
            cards_[i].value() == cards_[i+3].value())
          return true;
      }

      return false;
    }

    bool is_full_house() const {
      if (cards_[0].value() == cards_[1].value() &&
          cards_[2].value() == cards_[3].value() &&
          cards_[2].value() == cards_[4].value() &&
          cards_[0].value() != cards_[2].value())
        return true;
      else if (cards_[0].value() == cards_[1].value() &&
               cards_[0].value() == cards_[2].value() &&
               cards_[3].value() == cards_[4].value() &&
               cards_[0].value() != cards_[3].value())
        return true;
      else
        return false;
    }

    bool is_flush() const {
      for (unsigned int i = 1; i < cards_.size(); ++i) {
        if (cards_[i].suit() != cards_[0].suit())
          return false;
      }

      return true;
    }

    bool is_straight() const {
      for (unsigned int i = 0; i < cards_.size() - 1; ++i) {
        if (cards_[i].next_value() != cards_[i+1].value())
          return false;
      }

      return true;
    }

    bool is_three_of_a_kind() const {
      for (unsigned int i = 0; i < cards_.size() - 2; ++i) {
        if (cards_[i].value() == cards_[i+1].value() &&
            cards_[i].value() == cards_[i+2].value())
          return true;
      }

      return false;
    }

    bool is_two_pair() const {
      bool found_first_pair = false;
      char first_pair_val;

      for (unsigned int i = 0; i < cards_.size() - 1; ++i) {
        if (cards_[i].value() == cards_[i+1].value()) {
          if (!found_first_pair) {
            found_first_pair = true;
            first_pair_val = cards_[i].value();
          } else if (cards_[i].value() != first_pair_val) {
            return true;
          }
        }
      }

      return false;
    }

    bool is_one_pair() const {
      for (unsigned int i = 0; i < cards_.size() - 1; ++i) {
        // Since the cards are sorted, we can easily scan for pairs
        if (cards_[i].value() == cards_[i+1].value())
          return true;
      }

      return false;
    }

    friend std::ostream& operator<<(std::ostream& os, const Hand& h) {
      for (const auto& c : h.cards_)
        os << c << " ";

      return os;
    }

    // Assuming that this hand has a pair, return the highest value 
    Card get_highest_pair_value() const {
      Card highest_pair_value('2', 'S');
      for (unsigned int i = 0; i < cards_.size() - 1; ++i) {
        // Since the cards are sorted, we can easily scan for pairs
        if (cards_[i].value() == cards_[i+1].value() && highest_pair_value < cards_[i])
          highest_pair_value = cards_[i];
      }

      return highest_pair_value;
    }

    // Assuming that this hand has a three-of-a-kind, return the highest value 
    Card get_highest_triple_value() const {
      Card highest_triple_value('2', 'S');
      for (unsigned int i = 0; i < cards_.size() - 2; ++i) {
        // Since the cards are sorted, we can easily scan for pairs
        if (cards_[i].value() == cards_[i + 1].value() &&
            cards_[i].value() == cards_[i + 2].value() &&
            highest_triple_value < cards_[i])
          highest_triple_value = cards_[i];
      }

      return highest_triple_value;
    }

    // Assuming that this hand has a four-of-a-kind, return the highest value 
    Card get_highest_quadruple_value() const {
      Card highest_quad_value('2', 'S');
      for (unsigned int i = 0; i < cards_.size() - 3; ++i) {
        // Since the cards are sorted, we can easily scan for pairs
        if (cards_[i].value() == cards_[i + 1].value() &&
            cards_[i].value() == cards_[i + 2].value() &&
            cards_[i].value() == cards_[i + 3].value() &&
            highest_quad_value < cards_[i])
          highest_quad_value = cards_[i];
      }

      return highest_quad_value;
    }


    // Return true if a has the higher card, false otherwise
    friend bool has_higher_card(const Hand& a, const Hand& b) {
      for (int i = 4; i >= 0; --i) {
        if (b.cards_[i] < a.cards_[i])
          return true;
      }

      return false;
    }

    friend bool operator<(const Hand& a, const Hand& b) {
      if (a.is_royal_flush()) {
        if (b.is_royal_flush())
          return !has_higher_card(a, b);
        else
          return false;
      } else if (a.is_straight_flush()) {
        if (b.is_royal_flush())
          return true;
        else if (b.is_straight_flush()) 
          return !has_higher_card(a, b);
        else
          return false;
      } else if (a.is_four_of_a_kind()) {
        if (b.is_royal_flush() || b.is_straight_flush())
          return true;
        else if (b.is_four_of_a_kind()) {
          if (a.get_highest_quadruple_value() < b.get_highest_quadruple_value())
            return true;
          else if (b.get_highest_quadruple_value() < a.get_highest_quadruple_value())
            return false;
          else
            return !has_higher_card(a, b);
        }
        else
          return false;
      } else if (a.is_full_house()) {
        if (b.is_royal_flush() || b.is_straight_flush() ||
            b.is_four_of_a_kind())
          return true;
        else if (b.is_full_house()) {
          if (a.get_highest_triple_value() < b.get_highest_triple_value())
            return true;
          else if (b.get_highest_triple_value() < a.get_highest_triple_value())
            return false;
          else
            return !has_higher_card(a, b);
        } else
          return false;
      } else if (a.is_flush()) {
        if (b.is_royal_flush() || b.is_straight_flush() ||
            b.is_four_of_a_kind() || b.is_full_house())
          return true;
        else if (b.is_flush())
          return !has_higher_card(a, b);
        else
          return false;
      } else if (a.is_straight()) {
        if (b.is_royal_flush() || b.is_straight_flush() ||
            b.is_four_of_a_kind() || b.is_full_house() || b.is_flush())
          return true;
        else if (b.is_straight())
          return !has_higher_card(a, b);
        else
          return false;
      } else if (a.is_three_of_a_kind()) {
        if (b.is_royal_flush() || b.is_straight_flush() ||
            b.is_four_of_a_kind() || b.is_full_house() || b.is_flush() ||
            b.is_straight())
          return true;
        else if (b.is_three_of_a_kind()) {
          if (a.get_highest_triple_value() < b.get_highest_triple_value())
            return true;
          else if (b.get_highest_triple_value() < a.get_highest_triple_value())
            return false;
          else
            return !has_higher_card(a, b);
        } else
          return false;
      } else if (a.is_two_pair()) {
        if (b.is_royal_flush() || b.is_straight_flush() ||
            b.is_four_of_a_kind() || b.is_full_house() || b.is_flush() ||
            b.is_straight() || b.is_three_of_a_kind())
          return true;
        else if (b.is_two_pair()) {
          if (a.get_highest_pair_value() < b.get_highest_pair_value())
            return true;
          else if (b.get_highest_pair_value() < a.get_highest_pair_value())
            return false;
          else
            return !has_higher_card(a, b);
        } else
          return false;
      } else if (a.is_one_pair()) {
        if (b.is_royal_flush() || b.is_straight_flush() ||
            b.is_four_of_a_kind() || b.is_full_house() || b.is_flush() ||
            b.is_straight() || b.is_three_of_a_kind() || b.is_two_pair())
          return true;
        else if (b.is_one_pair()) {
          if (a.get_highest_pair_value() < b.get_highest_pair_value())
            return true;
          else if (b.get_highest_pair_value() < a.get_highest_pair_value())
            return false;
          else
            return !has_higher_card(a, b);
        } else
          return false;
      } else {
        if (b.is_royal_flush() || b.is_straight_flush() ||
            b.is_four_of_a_kind() || b.is_full_house() || b.is_flush() ||
            b.is_straight() || b.is_three_of_a_kind() || b.is_two_pair() ||
            b.is_one_pair())
          return true;
        else
          return !has_higher_card(a, b);
      }
    }

  private:
    std::vector<Card> cards_;
};

bool player_one_wins(std::string line) {
  std::vector<std::string> cards;
  size_t pos = 0;
  while ((pos = line.find(" ")) != std::string::npos) {
    std::string token = line.substr(0, pos);
    cards.push_back(token);
    line.erase(0, pos + 1);
  }

  // Last card
  cards.push_back(line);

  assert(cards.size() == 10);

  std::vector<Card> p1_cards;
  std::vector<Card> p2_cards;
  for (unsigned int i = 0; i < cards.size(); ++i) {
    if (i < 5)
      p1_cards.emplace_back(cards[i].at(0), cards[i].at(1));
    else
      p2_cards.emplace_back(cards[i].at(0), cards[i].at(1));
  }

  Hand p1(p1_cards);
  Hand p2(p2_cards);

  if (p2 < p1) {
    log_info("Player 1 wins with hand", p1, "against Player 2 with hand", p2);
  }

  return p2 < p1;
}

unsigned int compute_num_poker_wins(const std::string& filename) {
  std::ifstream f(filename);
  std::stringstream buf;
  buf << f.rdbuf();
  std::string all_lines = buf.str();

  std::vector<std::string> lines;
  size_t pos = 0;
  while ((pos = all_lines.find("\n")) != std::string::npos) {
    std::string token = all_lines.substr(0, pos);
    lines.push_back(token);
    all_lines.erase(0, pos + 1);
  }

  unsigned int num_player_one_wins = 0;
  for (auto& line : lines) {
    if (player_one_wins(line))
      ++num_player_one_wins;
  }

  return num_player_one_wins;
}

int main(int argc, char **argv) {
  if (argc < 2)
    throw std::runtime_error("Pass filename to read poker hands from.");

  std::cout << compute_num_poker_wins(argv[1]) << std::endl;
}
