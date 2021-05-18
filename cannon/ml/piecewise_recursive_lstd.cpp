#include <cannon/ml/piecewise_recursive_lstd.hpp>

#include <cassert>

#include <cannon/log/registry.hpp>

using namespace cannon::ml;
using namespace cannon::log;

// Public
void PiecewiseRecursiveLSTDFilter::process_datum(const VectorXd& in_vec, const VectorXd& next_in_vec,
    unsigned int idx, unsigned int next_idx, double reward) {
  RowVectorXd feat = make_feature_vec_(in_vec, idx);
  RowVectorXd next_feat = make_feature_vec_(next_in_vec, next_idx);
  RowVectorXd diff = feat - (discount_factor_ * next_feat);
  RowVectorXd g = diff * a_inv_;
  double a = 1.0 + g.transpose().dot(feat);
  double e_t = reward - diff.transpose().dot(theta_);
  VectorXd v = a_inv_ * feat.transpose();

  theta_ = theta_ + (e_t / a) * v;

  update_a_inv_(feat, next_feat);
}

VectorXd PiecewiseRecursiveLSTDFilter::get_mat(unsigned int idx) {
  unsigned int start_coord = idx * in_dim_;

  VectorXd ret_vec = theta_.segment(start_coord, in_dim_ - 1);
  return ret_vec;
}

double PiecewiseRecursiveLSTDFilter::predict(const VectorXd& in_vec, unsigned int idx) {
  RowVectorXd feat = make_feature_vec_(in_vec, idx);
  return feat * theta_;
}

void PiecewiseRecursiveLSTDFilter::reset() {
  a_inv_ = MatrixXd::Identity(param_dim_, param_dim_) * alpha_;
  theta_ = VectorXd::Zero(param_dim_);
}

// Private
RowVectorXd PiecewiseRecursiveLSTDFilter::make_feature_vec_(VectorXd in_vec, unsigned int idx) const {
  assert(in_vec.size() == in_dim_ - 1);
  assert(idx < num_refs_);

  RowVectorXd feat = RowVectorXd::Zero(param_dim_);
  unsigned int start_coord = idx * in_dim_;
  unsigned int end_coord = idx * in_dim_ + in_dim_ - 1;
  
  feat.segment(start_coord, in_dim_ - 1) = in_vec.transpose();
  feat(0, end_coord) = 1.0;

  return feat;
}

void PiecewiseRecursiveLSTDFilter::update_a_inv_(const RowVectorXd& feat, const RowVectorXd& next_feat) {
  assert(feat.size() == param_dim_);
  assert(next_feat.size() == param_dim_);

  RowVectorXd diff = feat - (discount_factor_ * next_feat);
  MatrixXd numerator = (a_inv_ * feat.transpose()) * (diff * a_inv_);
  assert(numerator.rows() == param_dim_);
  assert(numerator.cols() == param_dim_);

  MatrixXd denom_expr = diff * (a_inv_ * feat.transpose());
  assert(denom_expr.size() == 1);
  double denom = denom_expr(0, 0) + 1.0;

  MatrixXd update = numerator / denom;

  a_inv_ = (a_inv_ - update).eval();
}
