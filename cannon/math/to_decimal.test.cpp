#include <catch2/catch.hpp>

#include <cannon/math/to_decimal.hpp>

using namespace cannon::math;

TEST_CASE("ToDecimal", "[math]") {
  auto [nr, r] = compute_decimal_reciprocal(2);
  REQUIRE(nr.size() == 1);
  REQUIRE(nr[0] == 5);
  REQUIRE(r.size() == 0);

  auto [nr2, r2] = compute_decimal_reciprocal(3);
  REQUIRE(nr2.size() == 0);
  REQUIRE(r2.size() == 1);
  REQUIRE(r2[0] == 3);
}
