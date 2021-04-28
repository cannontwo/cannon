#include <catch2/catch.hpp>

#include <cannon/math/random_double.hpp>

using namespace cannon::math;

TEST_CASE("RandomDouble", "[math]") {

  for (unsigned int i = 0; i < 1000; i++) {
    double sample = random_double();
    REQUIRE(sample >= 0.0);
    REQUIRE(sample < 1.0);
  }

  for (unsigned int i = 0; i < 1000; i++) {
    double sample = random_double(10, 20);
    REQUIRE(sample >= 10.0);
    REQUIRE(sample < 20.0);
  }

}
