#include <cannon/geom/kd_tree.hpp>

using namespace cannon::geom;

int main() {
  KDTree<2> k;
  MatrixXd v(2, 2);
  v << 1.0, 2.0,
       3.0, 4.0;
  k.insert(v);

  // TODO Actually test
  assert(k.get_nearest_neighbor(v.col(0)) == v.col(0));
  assert(k.get_nearest_neighbor(v.col(1)) == v.col(1));
}
