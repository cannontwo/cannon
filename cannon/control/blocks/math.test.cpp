#include <catch2/catch.hpp>

#include <cannon/control/blocks/math.hpp>
#include <cannon/control/blocks/sources.hpp>
#include <cannon/control/out_port.hpp>
#include <cannon/control/signal.hpp>

using namespace cannon::control::blocks;

TEST_CASE("MathBlocks", "[control/blocks]") {
  Constant c1(1.0);
  auto s1 = c1.out_signal();

  Constant c2(2.0);
  auto s2 = c2.out_signal();

  Add add(s1, s2);
  add.update();
  REQUIRE(add.out_signal()->sample() == 3.0);
}
