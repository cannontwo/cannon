#include <cannon/ml/piecewise_lstd.hpp>

using namespace cannon::ml;

// Public
void PiecewiseLSTDFilter::process_datum(const VectorXd& in_vec, const VectorXd& next_in_vec,
    unsigned int idx, unsigned int next_idx, double reward) {
  RowVectorXd feat = make_feature_vec_(in_vec, idx);
  RowVectorXd next_feat = make_feature_vec_(next_in_vec, next_idx);

  update_a_inv_(feat, next_feat);
  update_b_(feat, reward);
  theta_updated_ = false;
}

VectorXd PiecewiseLSTDFilter::get_mat(unsigned int idx) {
  check_theta_();

  unsigned int start_coord = idx * in_dim_;

  VectorXd ret_vec(in_dim_ - 1);
  ret_vec = theta_.segment(start_coord, in_dim_ - 1);
  return ret_vec;
}

double PiecewiseLSTDFilter::predict(const VectorXd& in_vec, unsigned int idx) {
  check_theta_();

  RowVectorXd feat = make_feature_vec_(in_vec, idx);
  return feat * theta_;
}

void PiecewiseLSTDFilter::reset() {
  a_inv_ = MatrixXd::Identity(param_dim_, param_dim_) * alpha_;
  b_ = VectorXd::Zero(param_dim_);
  theta_ = VectorXd::Zero(param_dim_);
}

// Private
RowVectorXd PiecewiseLSTDFilter::make_feature_vec_(VectorXd in_vec, unsigned int idx) const {
  assert(in_vec.size() == in_dim_ - 1);
  assert(idx < num_refs_);

  RowVectorXd feat(param_dim_);
  unsigned int start_coord = idx * in_dim_;
  unsigned int end_coord = idx * in_dim_ + in_dim_ - 1;
  
  feat.segment(start_coord, in_dim_ - 1) = in_vec.transpose();
  feat(0, end_coord) = 1.0;

  return feat;
}

void PiecewiseLSTDFilter::update_a_inv_(const RowVectorXd& feat, const RowVectorXd& next_feat) {
  assert(feat.size() == param_dim_);
  assert(next_feat.size() == param_dim_);

  RowVectorXd diff = feat - (discount_factor_ * next_feat);
  MatrixXd numerator = a_inv_ * feat.transpose() * diff * a_inv_;
  assert(numerator.rows() == param_dim_);
  assert(numerator.cols() == param_dim_);

  MatrixXd denom_expr = diff * a_inv_ * feat.transpose();
  assert(denom_expr.size() == 1);
  double denom = denom_expr(0, 0) + 1.0;

  MatrixXd update = (1.0 / denom) * numerator;

  a_inv_ = a_inv_ - update;
}

void PiecewiseLSTDFilter::update_b_(const RowVectorXd& feat, double reward) {
  assert(feat.size() == param_dim_);

  b_ = b_ + (feat.transpose() * reward);
}

void PiecewiseLSTDFilter::check_theta_() {
  if (!theta_updated_) {
    theta_ = a_inv_ * b_;
    theta_updated_ = true;
  }
}
