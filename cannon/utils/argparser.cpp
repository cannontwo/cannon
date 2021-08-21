#include <cannon/utils/argparser.hpp>

using namespace cannon::utils;

void ArgParser::parse(int argc, char** argv) {
  // TODO Parse actual options
  
  inputs_.clear();
  for (int i = 1; i < argc; ++i)
    inputs_.push_back(std::string(argv[i]));
}

std::vector<std::string> ArgParser::get_inputs() const {
  return inputs_;
}
