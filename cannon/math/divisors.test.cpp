#include <catch2/catch.hpp>

#include <cannon/math/divisors.hpp>

using namespace cannon::math;

TEST_CASE("Divisors", "[math]") {
  REQUIRE(get_divisor_sum(10) == 8);
}
