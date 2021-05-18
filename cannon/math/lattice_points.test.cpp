#include <catch2/catch.hpp>

#include <cannon/math/lattice_points.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::math;
using namespace cannon::log;

TEST_CASE("LatticePoints", "[math]") {
  VectorXu one_d_sizes(1);
  one_d_sizes << 5;

  auto points = make_lattice_points(one_d_sizes);

  for (auto& p : points) {
    log_info(p);
  }

  REQUIRE(points.size() == 5);
  for (int i = 0; i < 5; i++) {
    VectorXu tmp = VectorXu::Zero(1);
    tmp[0] = i;

    REQUIRE(std::find(points.begin(), points.end(), tmp) != points.end());
  }

  VectorXu two_d_sizes(2);
  two_d_sizes << 5,
                 6;

  points = make_lattice_points(two_d_sizes);

  for (auto& p : points) {
    log_info(p);
  }

  REQUIRE(points.size() == 30);
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 6; j++) {
      VectorXu tmp = VectorXu::Zero(2);
      tmp[0] = i;
      tmp[1] = j;

      REQUIRE(std::find(points.begin(), points.end(), tmp) != points.end());
    }
  }
}
