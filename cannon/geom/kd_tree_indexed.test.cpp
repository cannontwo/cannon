#include <cannon/geom/kd_tree_indexed.hpp>

using namespace cannon::geom;

int main() {
  KDTreeIndexed k(2);
  MatrixXd v(2, 2);
  v << 1.0, 2.0,
       3.0, 4.0;
  k.insert(v);

  Vector2d q;
  q << 1.0, 
       2.0;

  assert(k.get_size() == 2);
  assert(k.get_nearest_neighbor(v.col(0)).second == 0 &&
      k.get_nearest_neighbor(v.col(0)).first == v.col(0));
  assert(k.get_nearest_neighbor(v.col(1)).second == 1 &&
      k.get_nearest_neighbor(v.col(1)).first == v.col(1));
  assert(k.get_nearest_neighbor(q).second == 0 &&
      k.get_nearest_neighbor(q).first == v.col(0));


  MatrixXd v2(2, 2);
  v2 << 1.0, 3.0,
       2.0, 4.0;
  k.insert(v2);

  assert(k.get_size() == 4);
  assert(k.get_nearest_neighbor(v.col(0)).second == 0 &&
      k.get_nearest_neighbor(v.col(0)).first == v.col(0));
  assert(k.get_nearest_neighbor(v.col(1)).second == 1 &&
      k.get_nearest_neighbor(v.col(1)).first == v.col(1));
  assert(k.get_nearest_neighbor(v2.col(0)).second == 2 &&
      k.get_nearest_neighbor(v2.col(0)).first == v2.col(0));
  assert(k.get_nearest_neighbor(v2.col(1)).second == 3 &&
      k.get_nearest_neighbor(v2.col(1)).first == v2.col(1));
  assert(k.get_nearest_neighbor(q).second == 2 &&
      k.get_nearest_neighbor(q).first == v2.col(0));

  k.clear();
  k.insert(q);
  assert(k.get_size() == 1);
  assert(k.get_nearest_neighbor(q).second == 0 &&
      k.get_nearest_neighbor(q).first == q);
  assert(k.get_nearest_idx(q) == 0);
}
