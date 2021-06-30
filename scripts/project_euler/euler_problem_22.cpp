#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <string>
#include <algorithm>

#include <cannon/log/registry.hpp>

using namespace cannon::log;

/*!
 * Using names.txt, a 46K text file containing over five-thousand first names,
 * begin by sorting it in alphabetical order. Then working out the alphabetical
 * value for each name, multiply this value by its alphabetical position in the
 * list to obtain a name score.
 *
 * For example, when the list is sorted into alphabetical order, COLIN, which
 * is worth 3 + 15 + 12 + 9 + 14 = 53, is the 938th name in the list. So, COLIN
 * would obtain a core of 938 * 53 = 49714.
 *
 * What is the total of all the name scores in this file?
 */

unsigned int get_alpha_score(const std::string& name) {
  // Assuming all letters are capitalized
  unsigned int score = 0;
  for (unsigned int i = 0; i < name.size(); ++i) {
    if (name.at(i) == '"')
      continue;

    log_info("Letter", name.at(i), "has value", static_cast<unsigned int>(name.at(i) - 'A') + 1);
    score += static_cast<unsigned int>(name.at(i) - 'A') + 1;
  }

  return score;
}

unsigned long compute_name_scores(const std::string& filename) {
  // Get names from file
  std::ifstream f(filename);
  std::stringstream buf;
  buf << f.rdbuf();
  std::string all_names = buf.str();

  // Extract names into vector
  // (https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c)
  std::vector<std::string> names;
  size_t pos = 0;
  while ((pos = all_names.find(",")) != std::string::npos) {
    std::string token = all_names.substr(0, pos);
    names.push_back(token);
    all_names.erase(0, pos + 1);
  }

  // Final name needs to be added
  names.push_back(all_names);

  // Sort names
  std::sort(names.begin(), names.end());

  // Compute name values
  unsigned long sum = 0;
  for (unsigned int i = 0; i < names.size(); i++) {
    unsigned int alpha_score = get_alpha_score(names[i]);
    unsigned int name_score = alpha_score * (i+1);

    log_info("Found name", names[i], "with score", name_score, "at position", i+1);

    sum += name_score;
  }

  return sum;
}

int main(int argc, char** argv) {
  if (argc < 1) {
    std::cerr << "Provide name file as argument" << std::endl;
    return 1;
  }
    
  std::cout << compute_name_scores(argv[1]) << std::endl;
}
