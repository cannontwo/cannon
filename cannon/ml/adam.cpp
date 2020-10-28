#include <cannon/ml/adam.hpp>

using namespace cannon::ml;

MatrixXd AdamOptimizer::apply_update(const MatrixXd& params, const MatrixXd& gradient) {
  assert(params.rows() == rows_ && params.cols() == cols_);  
  assert(gradient.rows() == rows_ && gradient.cols() == cols_);  

  t_ += 1;

  first_moments_ = (beta_1_ * first_moments_) + ((1.0 - beta_1_) * gradient);
  second_moments_ = (beta_2_ * second_moments_) + 
    ((1.0 - beta_2_) * gradient.array().pow(2.0).matrix());

  MatrixXd corr_first_moments = first_moments_ / (1.0 - std::pow(beta_1_, t_));
  MatrixXd corr_second_moments = second_moments_ / (1.0 - std::pow(beta_2_, t_));

  MatrixXd update = (corr_first_moments.array() /
      (corr_second_moments.array().pow(1.0/2.0) + epsilon_)).matrix();

  return params + (learning_rate_ * update);
}
