#include <catch2/catch.hpp>

#include <cannon/geom/kd_tree.hpp>

using namespace cannon::geom;

TEST_CASE("KDTree is consistent", "[geom]") {
  KDTree k(2);
  MatrixXd v(2, 2);
  v << 1.0, 2.0,
       3.0, 4.0;
  k.insert(v);

  REQUIRE(k.get_nearest_neighbor(v.col(0)) == v.col(0));
  REQUIRE(k.get_nearest_neighbor(v.col(1)) == v.col(1));
}
