#include <iostream>
#include <cassert>
#include <cmath>
#include <fstream>
#include <sstream>

#include <cannon/log/registry.hpp>

using namespace cannon::log;

/*!
 * Each character on a computer is assigned a unique code and the preferred
 * standard is ASCII (American Standard Code for Information Interchange). For
 * example, uppercase A = 65, asterisk (*) = 42, and lowercase k = 107.
 * 
 * A modern encryption method is to take a text file, convert the bytes to
 * ASCII, then XOR each byte with a given value, taken from a secret key. The
 * advantage with the XOR function is that using the same encryption key on
 * the cipher text, restores the plain text; for example, 65 XOR 42 = 107,
 * then 107 XOR 42 = 65.
 * 
 * For unbreakable encryption, the key is the same length as the plain text
 * message, and the key is made up of random bytes. The user would keep the
 * encrypted message and the encryption key in different locations, and
 * without both "halves", it is impossible to decrypt the message.
 * 
 * Unfortunately, this method is impractical for most users, so the modified
 * method is to use a password as a key. If the password is shorter than the
 * message, which is likely, the key is repeated cyclically throughout the
 * message. The balance for this method is using a sufficiently long password
 * key for security, but short enough to be memorable.
 * 
 * Your task has been made easy, as the encryption key consists of three lower
 * case characters. Using p059_cipher.txt (right click and 'Save Link/Target
 * As...'), a file containing the encrypted ASCII codes, and the knowledge
 * that the plain text must contain common English words, decrypt the message
 * and find the sum of the ASCII values in the original text.
 */

bool is_acceptable(char x) {
  return (('a' <= x && x <= 'z') || ('A' <= x && x <= 'Z') ||
          ('0' <= x && x <= '9') || x == ' ' || x == '\'' || x == ',' ||
          x == '"' || x == '[' || x == ']' || x == ':' || x == '/' ||
          x == '.' || x == '(' || x == ')' || x == ';' || x == '+');
}

unsigned int try_decrypt(const std::vector<int> &nums,
                         char a, char b, char c) {
  std::vector<char> code{a, b, c};
  unsigned int sum = 0;

  log_info("Trying to decrypt with secret:", code[0], code[1], code[2]);

  for (unsigned int i = 0; i < nums.size(); ++i) {
    char dec_char = nums[i] ^ code[i % code.size()];

    if (is_acceptable(dec_char)) {
      log_info("\tDecoded character", dec_char);
      sum += static_cast<unsigned int>(dec_char);
    } else
      return 0;
  }

  return sum;
} 

unsigned int decrypt_and_get_sum(const std::vector<int>& nums) {
  for (char a = 'a'; a <= 'z'; ++a) {
    for (char b = 'a'; b <= 'z'; ++b) {
      for (char c = 'a'; c <= 'z'; ++c) {

        unsigned int decrypt_sum = try_decrypt(nums, a, b, c);

        if (decrypt_sum > 0)
          return decrypt_sum;
      }
    }
  }

  return 0;
}

unsigned int get_ascii_sum(const std::string& filename) {
  std::ifstream f(filename);
  std::stringstream buf;
  buf << f.rdbuf();
  std::string all_nums = buf.str();

  // Extract numbers into vector
  // (https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c)
  std::vector<int> numbers;
  size_t pos = 0;
  while ((pos = all_nums.find(",")) != std::string::npos) {
    std::string token = all_nums.substr(0, pos);
    numbers.push_back(std::stoi(token));
    all_nums.erase(0, pos + 1);
  }

  // Final name needs to be added
  numbers.push_back(std::stoi(all_nums));

  return decrypt_and_get_sum(numbers);
}

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "Pass filename to read from." << std::endl;
    return 1;
  }

  std::cout << get_ascii_sum(argv[1]) << std::endl;
}
