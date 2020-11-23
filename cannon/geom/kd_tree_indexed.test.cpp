#include <catch2/catch.hpp>

#include <cannon/geom/kd_tree_indexed.hpp>

using namespace cannon::geom;

TEST_CASE("KDTree Indexed is consistent", "[geom]") {
  KDTreeIndexed k(2);
  MatrixXd v(2, 2);
  v << 1.0, 2.0,
       3.0, 4.0;
  k.insert(v);

  Vector2d q;
  q << 1.0, 
       2.0;

  REQUIRE(k.get_size() == 2);
  REQUIRE((k.get_nearest_neighbor(v.col(0)).second == 0 &&
      k.get_nearest_neighbor(v.col(0)).first == v.col(0)));
  REQUIRE((k.get_nearest_neighbor(v.col(1)).second == 1 &&
      k.get_nearest_neighbor(v.col(1)).first == v.col(1)));
  REQUIRE((k.get_nearest_neighbor(q).second == 0 &&
      k.get_nearest_neighbor(q).first == v.col(0)));


  MatrixXd v2(2, 2);
  v2 << 1.0, 3.0,
       2.0, 4.0;
  k.insert(v2);

  REQUIRE(k.get_size() == 4);
  REQUIRE((k.get_nearest_neighbor(v.col(0)).second == 0 &&
      k.get_nearest_neighbor(v.col(0)).first == v.col(0)));
  REQUIRE((k.get_nearest_neighbor(v.col(1)).second == 1 &&
      k.get_nearest_neighbor(v.col(1)).first == v.col(1)));
  REQUIRE((k.get_nearest_neighbor(v2.col(0)).second == 2 &&
      k.get_nearest_neighbor(v2.col(0)).first == v2.col(0)));
  REQUIRE((k.get_nearest_neighbor(v2.col(1)).second == 3 &&
      k.get_nearest_neighbor(v2.col(1)).first == v2.col(1)));
  REQUIRE((k.get_nearest_neighbor(q).second == 2 &&
      k.get_nearest_neighbor(q).first == v2.col(0)));

  k.clear();
  k.insert(q);
  REQUIRE(k.get_size() == 1);
  REQUIRE((k.get_nearest_neighbor(q).second == 0 &&
      k.get_nearest_neighbor(q).first == q));
  REQUIRE(k.get_nearest_idx(q) == 0);
}
