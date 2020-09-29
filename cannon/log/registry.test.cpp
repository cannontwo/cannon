#include <iostream>
#include <cassert>
#include <sstream>

#include <cannon/log/logger.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::log;

int main() {
  // Singleton behavior
  assert(&Registry::instance() == &Registry::instance());

  // Add logger
  std::stringstream ss; 
  auto l = std::make_shared<Logger>(ss);
  Registry::instance().add_logger(l);
  log("Test");
  assert(ss.str().compare("[INFO] Test\n") == 0);

  // Multiple loggers
  std::stringstream ss1, ss2;
  auto l1 = std::make_shared<Logger>(ss1);
  auto l2 = std::make_shared<Logger>(ss2);
  add_logger(l1);
  add_logger(l2);
  log("Test");
  assert(ss1.str().compare("[INFO] Test\n") == 0);
  assert(ss2.str().compare("[INFO] Test\n") == 0);

  // Log levels
  std::stringstream ss_info, ss_warning, ss_error;
  add_logger(ss_info);
  add_logger(ss_warning, Level::warning);
  add_logger(ss_error, Level::error);
  log_info("Test_info");
  log_warning("Test_warning");
  log_error("Test_error");

  std::cout << ss_info.str() << std::endl;
  std::cout << ss_warning.str() << std::endl;
  std::cout << ss_error.str() << std::endl;

  assert(ss_info.str().compare("[INFO] Test_info\n[WARN] Test_warning\n[ERR] Test_error\n") == 0);
  assert(ss_warning.str().compare("[WARN] Test_warning\n[ERR] Test_error\n") == 0);
  assert(ss_error.str().compare("[ERR] Test_error\n") == 0);
}
