#include <cannon/log/level.hpp>

using namespace cannon::log;

std::ostream& cannon::log::operator<<(std::ostream& os, const Level& l) {
  switch (l) {
    case info:
      os << "[INFO] ";
      break;
    case warning:
      os << "[WARN] ";
      break;
    case error:
      os << "[ERR] ";
      break;
  }
  return os;
}

