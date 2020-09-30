#include <cannon/log/logger.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::log;

// Logger member functions
void Registry::add_logger(std::shared_ptr<Logger> l) {
  loggers_.push_back(l);
}

// Free Functions
void cannon::log::add_logger(std::shared_ptr<Logger> l) {
  Registry::instance().add_logger(l);
}

void cannon::log::add_logger(std::ostream &os, const Level& level) {
  add_logger(std::make_shared<Logger>(os, level));
}
