#include <cannon/ml/rls.hpp>

#include <cassert>
#include <stdexcept>
#include <cmath>

#include <cannon/log/registry.hpp>

using namespace cannon::ml;
using namespace cannon::log;

// Public methods
void RLSFilter::process_datum(const VectorXd& in_vec, const VectorXd& output) {
  RowVectorXd feat = make_feature_vec_(in_vec);
  t_ += 1.0;

  // On the first update we don't have enough history to update everything else
  if (t_ == 1.0) {
    update_feat_mean_(feat);
    update_output_mean_(output);
    return;
  }

  MatrixX2d u = make_u_(feat);
  Matrix2Xd v = make_v_(feat);
  Matrix2d c = make_c_();
  MatrixXd c_t = u * c * v;

  update_covar_(u, c, v);
  update_output_mean_(output);
  update_feat_mean_(feat);
  update_theta_(c_t, feat, output);
  corrected_theta_ = theta_ - (((2.0 * t_) - 1) * (covar_ * feat_mean_.transpose() * output_mean_));
  intercept_ = (output_mean_ - (feat_mean_ * corrected_theta_)).transpose();

  update_pred_error_covar_(in_vec, output);
}

std::pair<MatrixXd, VectorXd> RLSFilter::get_identified_mats() const {
  return {corrected_theta_.transpose(), intercept_};
}

MatrixXd RLSFilter::get_pred_error_covar() const {
  return pred_error_covar_;
}

VectorXd RLSFilter::predict(const VectorXd& in_vec) {
  RowVectorXd feat = make_feature_vec_(in_vec);
  VectorXd prediction = (feat * corrected_theta_).transpose() + intercept_;
  assert(prediction.size() == out_dim_);
  return prediction;
}

void RLSFilter::reset() {
  t_ = 0.0;
  intercept_ = VectorXd::Zero(out_dim_);

  theta_ = MatrixXd::Zero(param_dim_, out_dim_);
  corrected_theta_ = MatrixXd::Zero(param_dim_, out_dim_);

  feat_mean_ = RowVectorXd::Zero(param_dim_);
  output_mean_ = RowVectorXd::Zero(out_dim_);

  covar_ = MatrixXd::Identity(param_dim_, param_dim_) * alpha_;
  pred_error_covar_ = MatrixXd::Zero(out_dim_, out_dim_);
}

void RLSFilter::set_params(const Ref<const MatrixXd> &theta,
                           const Ref<const VectorXd> &intercept,
                           const Ref<const VectorXd> &in_mean) {
  
  if (theta.rows() != param_dim_ || theta.cols() != out_dim_)
    throw std::runtime_error("Theta matrix has the wrong size");

  if (intercept.size() == out_dim_)
    throw std::runtime_error("Intercept has the wrong size");

  if (intercept.size() == param_dim_)
    throw std::runtime_error("Intercept has the wrong size");

  reset();

  theta_ = theta;
  corrected_theta_ = theta_;
  intercept_ = intercept;
  feat_mean_ = in_mean.transpose();
  output_mean_ = intercept_ + feat_mean_ * theta_;
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

Matrix2d RLSFilter::make_c_() const {
  Matrix2d ret_mat;
  ret_mat << std::pow((2.0 * t_) - 1.0, 2.0) - (2.0 * std::pow(t_, 2.0)),
             -((2.0 * t_) - 1.0) * (t_ - 1.0),
             -((2.0 * t_) - 1.0) * (t_ - 1.0),
             std::pow(t_ - 1.0, 2.0);

  return ret_mat * (1.0 / (std::pow(forgetting_ * t_, 2.0)));
}

void RLSFilter::update_covar_(const MatrixX2d& u, const Matrix2d& c, 
    const Matrix2Xd& v) {
  if (u.rows() != param_dim_)
    throw std::runtime_error("Passed U matrix has the wrong number of rows");
  if (v.cols() != param_dim_)
    throw std::runtime_error("Passed V matrix has the wrong number of columns");


  Matrix2d inv_part = c.inverse() + (v * covar_ * u);
  MatrixXd update = covar_ * u * inv_part.inverse() * v * covar_;
  covar_ = ((1.0 / std::pow(forgetting_, 2.0)) * (covar_ - update)).eval();
}

void RLSFilter::update_theta_(const MatrixXd& c_t, const RowVectorXd& feat, 
    const VectorXd& output) {
  if (c_t.rows() != param_dim_ || c_t.cols() != param_dim_)
    throw std::runtime_error("Passed c_t matrix has the wrong shape");
  if (feat.size() != param_dim_)
    throw std::runtime_error("Passed feature has wrong size");
  if (output.size() != out_dim_)
    throw std::runtime_error("Passed output has wrong size");

  MatrixXd inner_term = (feat.transpose() * output.transpose()) - (c_t * theta_);
  MatrixXd update = covar_ * inner_term;
  theta_ = (theta_ + update).eval();
}

void RLSFilter::update_output_mean_(const VectorXd& output) {
  if (output.size() != out_dim_)
    throw std::runtime_error("Passed output has wrong size");

  output_mean_ = ((forgetting_ * output_mean_) + ((1.0 / t_) *
      (output.transpose() - (forgetting_ * output_mean_)))).eval();
}

void RLSFilter::update_feat_mean_(const RowVectorXd& feat) {
  if (feat.size() != param_dim_)
    throw std::runtime_error("Passed feat has wrong size");

  feat_mean_ = ((forgetting_ * feat_mean_) + ((1.0 / t_) * (feat - (forgetting_ * feat_mean_)))).eval();
}

void RLSFilter::update_pred_error_covar_(const VectorXd& in_vec, const VectorXd& output) {
  if (in_vec.size() != param_dim_)
    throw std::runtime_error("Passed in_vec has wrong size");
  if (output.size() != out_dim_)
    throw std::runtime_error("Passed output has wrong size");

  VectorXd pred = predict(in_vec);
  VectorXd err = output - pred;
  assert(err.size() == out_dim_);

  pred_error_covar_ = ((forgetting_ * pred_error_covar_) + ((1.0 / t_) * ((err *
          err.transpose()) - (forgetting_ * pred_error_covar_)))).eval();
}

unsigned int RLSFilter::get_num_data() const {
  assert(t_ >= 0.0);

  return (unsigned int)t_;
}
