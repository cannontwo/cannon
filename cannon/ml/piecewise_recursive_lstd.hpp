#ifndef CANNON_ML_RECURSIVE_PIECEWISE_LSTD_H
#define CANNON_ML_RECURSIVE_PIECEWISE_LSTD_H 

#include <cassert>

#include <Eigen/Core>

#include <cannon/log/registry.hpp>

using namespace Eigen;

using namespace cannon::log;

namespace cannon {
  namespace ml {

    class PiecewiseRecursiveLSTDFilter {
      public:
        PiecewiseRecursiveLSTDFilter() = delete;

        PiecewiseRecursiveLSTDFilter(unsigned int in_dim, unsigned int num_refs, double
            discount_factor, double alpha=1.0) : in_dim_(in_dim + 1),
        param_dim_(in_dim_ * num_refs), num_refs_(num_refs),
        discount_factor_(discount_factor), alpha_(alpha) {
          a_inv_ = MatrixXd::Identity(param_dim_, param_dim_) * alpha_;
          theta_ = VectorXd::Zero(param_dim_);
        }

        void process_datum(const VectorXd& in_vec, const VectorXd& next_in_vec,
            unsigned int idx, unsigned int next_idx, double reward);

        VectorXd get_mat(unsigned int idx);

        double predict(const VectorXd& in_vec, unsigned int idx);

        void reset();

      private:
        RowVectorXd make_feature_vec_(VectorXd in_vec, unsigned int idx) const;

        void update_a_inv_(const RowVectorXd& feat, const RowVectorXd& next_feat);

        // Parameters
        unsigned int in_dim_;
        unsigned int param_dim_;
        unsigned int num_refs_;
        double discount_factor_;
        double alpha_;

        // Matrices
        MatrixXd a_inv_;
        VectorXd theta_;
    };
    
  } // namespace ml
} // namespace cannon
#endif /* ifndef CANNON_ML_RECURSIVE_PIECEWISE_LSTD_H */
