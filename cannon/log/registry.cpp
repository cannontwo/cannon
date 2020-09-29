#include <cannon/log/logger.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::log;

// Logger member functions
void Registry::add_logger(std::shared_ptr<Logger> l) {
  loggers_.push_back(l);
}

void Registry::log(const std::string& s, const Level& level) {
  for (auto l : loggers_) {
    l->log(s, level);
  }
}

// Free Functions
void cannon::log::add_logger(std::shared_ptr<Logger> l) {
  Registry::instance().add_logger(l);
}

void cannon::log::add_logger(std::ostream &os, const Level& level) {
  add_logger(std::make_shared<Logger>(os, level));
}

void cannon::log::log(const std::string &s, const Level& level) {
  Registry::instance().log(s, level);
}

void cannon::log::log_info(const std::string &s) {
  log(s, Level::info);
}

void cannon::log::log_warning(const std::string &s) {
  log(s, Level::warning);
}

void cannon::log::log_error(const std::string &s) {
  log(s, Level::error);
}
