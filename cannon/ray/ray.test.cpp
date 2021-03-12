#include <catch2/catch.hpp>

#include <cannon/ray/ray.hpp>

using namespace cannon::ray;

TEST_CASE("Ray", "[ray]") {
  Vector3d o = Vector3d::Zero();
  Vector3d d = Vector3d::Ones();
  
  Ray r(o, d);
  REQUIRE(r.at(0.5)[0] == 0.5);
  REQUIRE(r.at(0.5)[1] == 0.5);
  REQUIRE(r.at(0.5)[2] == 0.5);


}
