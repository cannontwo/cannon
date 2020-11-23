#include <catch2/catch.hpp>

#include <string>
#include <sstream>
#include <iostream>

#include <cannon/log/logger.hpp>

using namespace cannon::log;

TEST_CASE("Logger", "[log]") {
  // Basic test
  std::stringstream ss1;
  Logger l1(ss1);
  l1.log(Level::info, "Test");
  REQUIRE(ss1.str().compare("[INFO] Test\n") == 0);

  // Logging disabled by level
  std::stringstream ss2;
  Logger l2(ss2, Level::warning);

  l2.log(Level::info, "Test1");
  l2.log(Level::warning, "Test2");
  l2.log(Level::error, "Test3");

  REQUIRE(ss2.str().compare("[WARN] Test2\n[ERR] Test3\n") == 0);

  // Set logging level
  std::stringstream ss3;
  Logger l3(ss3);
  l3.set_level(Level::error);
  l3.log(Level::info, "Test");
  REQUIRE(ss3.str().compare("") == 0);
}
