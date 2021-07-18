#include <cannon/ml/piecewise_recursive_lstd.hpp>

#include <cassert>

#include <cannon/log/registry.hpp>

using namespace cannon::ml;
using namespace cannon::log;

// Public
void PiecewiseRecursiveLSTDFilter::process_datum(const VectorXd& in_vec, const VectorXd& next_in_vec,
    unsigned int idx, unsigned int next_idx, double reward) {
  auto feat = make_feature_vec_(in_vec, idx);
  auto next_feat = make_feature_vec_(next_in_vec, next_idx);
  SparseMatrix<double> diff = feat - (discount_factor_ * next_feat);

  auto g = diff * a_inv_;
  
  MatrixXd tmp = g * feat.transpose();
  assert(tmp.size() == 1);
  double a = tmp(0, 0) + 1.0;

  MatrixXd tmp2 = diff * theta_;
  assert(tmp2.size() == 1);
  double e_t = reward - tmp2(0, 0);

  VectorXd v = a_inv_ * feat.transpose();
  theta_ = theta_ + (e_t / a) * v;

  update_a_inv_(feat, next_feat);
}

VectorXd PiecewiseRecursiveLSTDFilter::get_mat(unsigned int idx) const {
  unsigned int start_coord = idx * in_dim_;

  VectorXd ret_vec = theta_.segment(start_coord, in_dim_ - 1);
  return ret_vec;
}

double PiecewiseRecursiveLSTDFilter::predict(const VectorXd& in_vec, unsigned int idx) const {
  RowVectorXd feat = make_feature_vec_(in_vec, idx);
  return feat * theta_;
}

void PiecewiseRecursiveLSTDFilter::reset() {
  a_inv_ = MatrixXd::Identity(param_dim_, param_dim_) * alpha_;
  theta_ = VectorXd::Zero(param_dim_);
}

// Private
SparseMatrix<double>
PiecewiseRecursiveLSTDFilter::make_feature_vec_(VectorXd in_vec,
                                                unsigned int idx) const {
  assert(in_vec.size() == in_dim_ - 1);
  assert(idx < num_refs_);

  SparseMatrix<double> feat(1, param_dim_);
  unsigned int start_coord = idx * in_dim_;
  unsigned int end_coord = idx * in_dim_ + in_dim_ - 1;
  
  std::vector<Triplet<double>> triplets;
  triplets.reserve(in_vec.size());
  for (unsigned int i = 0; i < in_vec.size(); ++i) {
    triplets.push_back(Triplet<double>(0, start_coord+i, in_vec[i]));
  }
  triplets.push_back(Triplet<double>(0, end_coord, 1.0));
  feat.setFromTriplets(triplets.begin(), triplets.end());

  return feat;
}

void PiecewiseRecursiveLSTDFilter::update_a_inv_(
    const SparseMatrix<double> &feat,
    const SparseMatrix<double> &next_feat) {
  assert(feat.rows() == 1);
  assert(feat.cols() == param_dim_);
  assert(next_feat.rows() == 1);
  assert(next_feat.cols() == param_dim_);

  SparseMatrix<double> diff = feat - (discount_factor_ * next_feat);

  MatrixXd numerator = (a_inv_ * feat.transpose()) * (diff * a_inv_);
  assert(numerator.rows() == param_dim_);
  assert(numerator.cols() == param_dim_);

  MatrixXd denom_expr = diff * (a_inv_ * feat.transpose());
  assert(denom_expr.size() == 1);
  double denom = denom_expr(0, 0) + 1.0;

  MatrixXd update = numerator / denom;
  a_inv_ = (a_inv_ - update).eval();
}
