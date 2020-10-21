#include <cannon/ml/rls.hpp>

using namespace cannon::ml;

// Public methods
void RLSFilter::process_datum(const VectorXd& in_vec, const VectorXd& output) {
  // TODO
}

std::pair<MatrixXd, VectorXd> RLSFilter::get_identified_mats() const {
  return {corrected_theta_.transpose(), intercept_};
}

VectorXd RLSFilter::predict(const VectorXd& in_vec) {
  // TODO
}

void reset() {
  // TODO
}

// Private methods
RowVectorXd RLSFilter::make_feature_vec_(const VectorXd& in_vec) const {
  if (in_vec.size() != in_dim_)
   throw std::runtime_error("Dimension of passed vector doesn't match dimension of RLS"); 

  return in_vec.transpose();
}

MatrixX2d RLSFilter::make_u_(const RowVectorXd& feat) const {
  if (feat.size() != param_dim_)
    throw std::runtime_error("Dimension of passed feature doesn't match dimension of RLS");

  MatrixX2d ret_mat(param_dim_, 2);
  ret_mat.col(0) = feat_mean_.transpose() * forgetting_;
  ret_mat.col(1) = feat.transpose();

  return ret_mat;
}

Matrix2Xd RLSFilter::make_v_(const RowVectorXd& feat) const {
  if (feat.size() != param_dim_)
    throw std::runtime_error("Dimension of passed feature doesn't match dimension of RLS");

  Matrix2Xd ret_mat(2, param_dim_);
  ret_mat.row(0) = feat_mean_ * forgetting_;
  ret_mat.row(1) = feat;

  return ret_mat;
}

Matrix2d RLSFilter::make_c_(const RowVectorXd& feat) const {
  if (feat.size() != param_dim_)
    throw std::runtime_error("Dimension of passed feature doesn't match dimension of RLS");

  Matrix2d ret_mat;
  ret_mat << std::pow((2.0 * t_) - 1.0, 2.0) - (2.0 * std::pow(t_, 2.0)),
             -((2.0 * t_) - 1.0) * (t_ - 1.0),
             -((2.0 * t_) - 1.0) * (t_ - 1.0),
             std::pow(t_ - 1.0, 2.0);

  return ret_mat;
}

void RLSFilter::update_covar_(const MatrixX2d& u, const Matrix2d& c, 
    const Matrix2Xd& v) {
  // TODO
}

void RLSFilter::update_theta_(const MatrixXd& c_t, const RowVectorXd& feat, 
    const VectorXd& output) {
  // TODO
}

void RLSFilter::update_output_mean_(const VectorXd& output) {
  // TODO
}

void RLSFilter::update_feat_mean_(const RowVectorXd& feat) {
  // TODO
}

void RLSFilter::update_pred_error_covar_(const VectorXd& in_vec, const VectorXd& output) {
  // TODO
}

