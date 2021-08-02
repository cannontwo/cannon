#include <catch2/catch.hpp>

#include <cannon/math/interp.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::math;
using namespace cannon::log;

TEST_CASE("Interp", "[math]") {
  auto pts = cheb_points(10);

  log_info("10 Cheb points on [-1, 1] are:");
  for (auto& x : pts)
    log_info("\t", x);



}
