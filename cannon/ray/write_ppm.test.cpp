#include <sstream>

#include <catch2/catch.hpp>

#include <cannon/ray/write_ppm.hpp>

using namespace cannon::ray;

TEST_CASE("WritePPM", "[ray]") {
  std::stringstream ss;

  Vector3d c1 = Vector3d::Zero();
  write_color(ss, c1);
  REQUIRE(ss.str().compare("0 0 0\n") == 0);
  ss.str("");

  Vector3d c2 = Vector3d::Ones();
  write_color(ss, c2);
  REQUIRE(ss.str().compare("255 255 255\n") == 0);
  ss.str("");

}
