#include <catch2/catch.hpp>

#include <cannon/physics/lagrangian_mechanics.hpp>
#include <cannon/math/nearly_equal.hpp>

using namespace cannon::physics;
using namespace cannon::math;

double L(const LocalTuple& local) { 
  assert(local.coordinates.size() == 3);

  double m = 3.0;

  return (1.0 / 2.0) * m * local.velocities.squaredNorm();
}

TEST_CASE("LagrangianMechanics", "[physics]") {
  Vector3d start;
  start[0] = 7;
  start[1] = 5;
  start[2] = 1;

  Vector3d end;
  end[0] = 47;
  end[1] = 35;
  end[2] = 21;

  Path q(0.0, 10.0, start, end, 100);
  log_info("Action is", compute_path_action(L, q));

  REQUIRE(nearly_equal(compute_path_action(L, q), 435.0));
}
