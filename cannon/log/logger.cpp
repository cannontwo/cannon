#include <cannon/log/logger.hpp>

using namespace cannon::log;

void Logger::set_level(const Level& l) {
  level_ = l;
}
