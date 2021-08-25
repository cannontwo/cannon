#include <catch2/catch.hpp>

#include <cannon/geom/trajectory.hpp>

using namespace cannon::geom;

TEST_CASE("Trajectory", "[geom]") {
  Trajectory traj;

  traj.push_back(VectorXd::Zero(2), 0.0);
  traj.push_back(VectorXd::Ones(2), 1.0);

  REQUIRE(traj(-1.0) == VectorXd::Zero(2));
  REQUIRE(traj(2.0) == VectorXd::Ones(2));
  REQUIRE(traj(0.5) == VectorXd::Ones(2) * 0.5);

}
