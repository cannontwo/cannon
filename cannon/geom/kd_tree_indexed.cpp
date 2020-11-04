#include <cannon/geom/kd_tree_indexed.hpp>

using namespace cannon::geom;

void KDTreeIndexed::insert(const MatrixXd& pts) {
  if (pts.rows() != dim_)
    throw std::runtime_error("Matrix of points to be inserted has the wrong number of rows.");

  for (int i = 0; i < pts.cols(); i++) {
    auto col = pts.col(i);
    Point_d tmp(col.data(), col.data() + col.size());
    log_info("Inserting point", tmp);

    Point_and_int tmp_pair = boost::make_tuple(tmp, furthest_idx_ + i);
    tree_.insert(tmp_pair);
  }

  furthest_idx_ += pts.cols();
}

std::pair<VectorXd, int> KDTreeIndexed::get_nearest_neighbor(const VectorXd& query) {
  if (query.size() != dim_)
    throw std::runtime_error("Query vector has the wrong number of rows.");

  Point_d tmp_q(query.data(), query.data() + query.size());
  NeighborSearch search(tree_, tmp_q, 1);

  // Check that we have something to return
  if (search.begin() == search.end())
    throw std::runtime_error("KDTree search didn't return anything.");

  auto ret_pair = search.begin()->first;
  Point_d ret_pt = boost::get<0>(ret_pair);

  VectorXd ret_vec(dim_);
  for (int i = 0; i < dim_; i++) {
    ret_vec[i] = ret_pt[i];
  }

  return std::make_pair(ret_vec, boost::get<1>(ret_pair));
}

int KDTreeIndexed::get_nearest_idx(const VectorXd& query) {
  assert(query.size() == dim_);
  Point_d tmp_q(query.data(), query.data() + query.size());
  NeighborSearch search(tree_, tmp_q, 1);

  // Check that we have something to return
  if (search.begin() == search.end())
    throw std::runtime_error("KDTree search didn't return anything.");

  auto ret_pair = search.begin()->first;
  return boost::get<1>(ret_pair);
}

void KDTreeIndexed::clear() {
  tree_.clear();
  furthest_idx_ = 0;
}

int KDTreeIndexed::get_size() {
  return tree_.size();
}
