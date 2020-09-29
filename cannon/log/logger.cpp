#include <cannon/log/logger.hpp>

using namespace cannon::log;

void Logger::log(const std::string& s, const Level& l) {
  if (l >= level_) {
    os_ << l << s << std::endl;
  }
}

void Logger::set_level(const Level& l) {
  level_ = l;
}
