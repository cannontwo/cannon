#include <cannon/ml/piecewise_ilstd.hpp>

#include <cannon/log/registry.hpp>

using namespace cannon::ml;
using namespace cannon::log;

// Public Methods

void PiecewiseILSTDFilter::process_datum(const VectorXd &in_vec,
                                         const VectorXd &next_in_vec,
                                         unsigned int idx,
                                         unsigned int next_idx, double reward) {
  RowVectorXd feat = make_feature_vec_(in_vec, idx);
  RowVectorXd next_feat = make_feature_vec_(next_in_vec, next_idx);

  RowVectorXd delta_b = feat * reward;

  RowVectorXd diff = feat - (discount_factor_ * next_feat);
  MatrixXd delta_a = feat.transpose() * diff;

  a_ = (a_ + delta_a).eval();
  VectorXd data_mu_update = delta_b.transpose() - (delta_a * theta_);
  log_info("Data-based Mu update is", data_mu_update);


  mu_ = (mu_ + data_mu_update).eval();

  log_info("Mu is", mu_);

  // Incremental theta updates
  for (unsigned int i = 0; i < num_updates_; i++) {
    VectorXd::Index j;
    double max = mu_.cwiseAbs().maxCoeff(&j);

    theta_[j] = theta_[j] + alpha_ * mu_[j];

    log_info("Mu update is", alpha_ * mu_[j] * a_.col(j));
    log_info("Column", j, "of A is", a_.col(j));
    VectorXd mu_update = alpha_ * mu_[j] * a_.col(j);

    if (mu_update.norm() >= 1e-16)
      mu_ = (mu_ - mu_update).eval();
  }
}


VectorXd PiecewiseILSTDFilter::get_mat(unsigned int idx) const {
  unsigned int start_coord = idx * in_dim_;

  VectorXd ret_vec = theta_.segment(start_coord, in_dim_ - 1);
  return ret_vec;
}

double PiecewiseILSTDFilter::predict(const VectorXd& in_vec, unsigned int idx) const {
  RowVectorXd feat = make_feature_vec_(in_vec, idx);
  return feat * theta_;
}

void PiecewiseILSTDFilter::reset() {
  a_ = MatrixXd::Zero(param_dim_, param_dim_) * alpha_;
  mu_ = VectorXd::Zero(param_dim_);
  //theta_ = VectorXd::Zero(param_dim_);
}

// Private Methods
RowVectorXd PiecewiseILSTDFilter::make_feature_vec_(VectorXd in_vec, unsigned int idx) const {
  assert(in_vec.size() == in_dim_ - 1);
  assert(idx < num_refs_);

  RowVectorXd feat = RowVectorXd::Zero(param_dim_);
  unsigned int start_coord = idx * in_dim_;
  unsigned int end_coord = idx * in_dim_ + in_dim_ - 1;
  
  feat.segment(start_coord, in_dim_ - 1) = in_vec.transpose();
  feat(0, end_coord) = 1.0;

  return feat;
}
