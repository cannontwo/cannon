#include <catch2/catch.hpp>

#include <iomanip>

#include <cannon/math/rootfinding.hpp>
#include <cannon/math/nearly_equal.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::math;
using namespace cannon::log;

TEST_CASE("Rootfinding", "[math]") {
  double r = bisection_method([](double x){
      return x * x - 10;
      }, -10, 0);
  log_info(std::setprecision(15), r);
  REQUIRE(nearly_equal(r, -std::sqrt(10)));

  r = bisection_method([](double x){
      return x * x - 10;
      }, 0, 10);
  log_info(std::setprecision(15), r);
  REQUIRE(nearly_equal(r, std::sqrt(10)));

  r = regula_falsi([](double x){
      return x * x - 10;
      }, -10, 0);
  log_info(std::setprecision(15), r);
  REQUIRE(nearly_equal(r, -std::sqrt(10)));

  r = regula_falsi([](double x){
      return x * x - 10;
      }, 0, 10);
  log_info(std::setprecision(15), r);
  REQUIRE(nearly_equal(r, std::sqrt(10)));

  r = newton_method([](double x) { return x * x - 10; },
                    [](double x) { return 2 * x; }, -10);
  log_info(std::setprecision(15), r);
  REQUIRE(nearly_equal(r, -std::sqrt(10)));
}
