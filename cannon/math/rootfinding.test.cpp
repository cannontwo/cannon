#include <catch2/catch.hpp>

#include <cannon/math/rootfinding.hpp>
#include <cannon/math/nearly_equal.hpp>

using namespace cannon::math;

TEST_CASE("Rootfinding", "[math]") {
  double r = bisection_method([](double x){
      return x * x - 10;
      }, -10, 0);
  REQUIRE(nearly_equal(r, -std::sqrt(10)));

  r = bisection_method([](double x){
      return x * x - 10;
      }, 0, 10);
  REQUIRE(nearly_equal(r, std::sqrt(10)));
}
