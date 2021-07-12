#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>
#include <sstream>

#include <cannon/log/registry.hpp>

using namespace cannon::log;

/*!
 * The nth term of the sequence of triangle numbers is given by t_n = 1/2 * (n
 * * (n+1)); so the first ten triangle numbers are:
 *
 *   1, 3, 6, 10, 15, 21, 28, 36, 45, 55
 *
 * By converting each letter in a word to a number corresponding to its
 * alphabetical position and adding these values we form a word value. For
 * example, the word value for SKY is 19 + 11 + 25 = 55 = t_10. If the word
 * value is a triangle number then we shall call the word a triangle word. 
 *
 * How many words in the input file are triangle words?
 */

unsigned int get_alpha_score(const std::string& word) {
  unsigned int score = 0;
  for (unsigned int i = 0; i < word.size(); ++i) {
    if (word.at(i) == '"')
      continue;

    log_info("Letter", word.at(i), "has value", static_cast<unsigned int>(word.at(i) - 'A') + 1);
    score += static_cast<unsigned int>(word.at(i) - 'A') + 1;
  }

  return score;
}

bool is_triangle_num(unsigned int x) {
  for (unsigned int i = 0; i < std::sqrt(2*x); ++i) {
    if ((i * (i+1) / 2) == x)
      return true;
  }

  return false;
}

unsigned int compute_num_triangle_words(const std::string& filename) {
  // Get words from file
  std::ifstream f(filename);
  std::stringstream buf;
  buf << f.rdbuf();
  std::string all_words = buf.str();

  // Extract words into vector
  std::vector<std::string> words;
  size_t pos = 0;
  while ((pos = all_words.find(",")) != std::string::npos) {
    std::string token = all_words.substr(0, pos);
    words.push_back(token);
    all_words.erase(0, pos+1);
  }

  // Final word has to be added as well
  words.push_back(all_words);

  unsigned int num_triangle = 0;
  for (const auto& word : words) {
    auto score = get_alpha_score(word);

    if (is_triangle_num(score))
      ++num_triangle;
  }

  return num_triangle;
}

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cerr << "Pass filename to read from" << std::endl;
    return 1;
  }

  std::cout << compute_num_triangle_words(argv[1]) << std::endl;
}
