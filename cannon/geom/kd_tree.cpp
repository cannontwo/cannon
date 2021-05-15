#include <stdexcept>

#include <cannon/geom/kd_tree.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::geom;
using namespace cannon::log;

void KDTree::insert(const MatrixXd& pts) {
  if (pts.rows() != dim_)
    throw std::runtime_error("Matrix of points to be inserted has the wrong number of rows.");

  for (int i = 0; i < pts.cols(); i++) {
    auto col = pts.col(i);
    Point_d tmp(col.data(), col.data() + col.size());
    log_info("Inserting point", tmp);
    tree_.insert(tmp);
  }
}

VectorXd KDTree::get_nearest_neighbor(const VectorXd& query) {
  if (query.size() != dim_)
    throw std::runtime_error("Query vector has the wrong number of rows.");
  Point_d tmp_q(query.data(), query.data() + query.size());
  NeighborSearch search(tree_, tmp_q, 1);

  // Check that we have something to return
  if (search.begin() == search.end())
    throw std::runtime_error("KDTree search didn't return anything.");

  Point_d ret_pt = search.begin()->first;
  VectorXd ret_vec(dim_);
  for (int i = 0; i < dim_; i++) {
    ret_vec[i] = ret_pt[i];
  }

  return ret_vec;
}

void KDTree::clear() {
  tree_.clear();
}

int KDTree::get_size() {
  return tree_.size();
}

