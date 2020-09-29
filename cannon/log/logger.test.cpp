#include <cassert>
#include <string>
#include <sstream>
#include <iostream>

#include <cannon/log/logger.hpp>

using namespace cannon::log;

int main() {
  // Basic test
  std::stringstream ss1;
  Logger l1(ss1);
  l1.log("Test");
  assert(ss1.str().compare("[INFO] Test\n") == 0);

  // Logging disabled by level
  std::stringstream ss2;
  Logger l2(ss2, Level::warning);

  l2.log("Test1", Level::info);
  l2.log("Test2", Level::warning);
  l2.log("Test3", Level::error);

  assert(ss2.str().compare("[WARN] Test2\n[ERR] Test3\n") == 0);

  // Set logging level
  std::stringstream ss3;
  Logger l3(ss3);
  l3.set_level(Level::error);
  l3.log("Test", Level::info);
  assert(ss3.str().compare("") == 0);
  
  return 0;
}
