#ifndef CANNON_ML_RLS_H
#define CANNON_ML_RLS_H 

#include <utility>
#include <stdexcept>
#include <cmath>
#include <cassert>

#include <Eigen/Dense>

#include <cannon/log/registry.hpp>

using namespace Eigen;

using namespace cannon::log;

namespace cannon {
  namespace ml {

    // A Recursive Least Squares Filter supporting intercept estimation and
    // forgetting. See http://cannontwo.com/assets/rls_notes.pdf.
    class RLSFilter {
      public:

        RLSFilter() = delete;

        RLSFilter(unsigned int in_dim, unsigned int out_dim, double alpha=1.0e6,
            double forgetting=1.0) : in_dim_(in_dim), out_dim_(out_dim),
        param_dim_(in_dim), alpha_(alpha), forgetting_(forgetting) {
          intercept_ = VectorXd::Zero(out_dim_);

          theta_ = MatrixXd::Zero(param_dim_, out_dim_);
          corrected_theta_ = MatrixXd::Zero(param_dim_, out_dim_);

          feat_mean_ = RowVectorXd::Zero(param_dim_);
          output_mean_ = RowVectorXd::Zero(out_dim_);

          covar_ = MatrixXd::Identity(param_dim_, param_dim_) * alpha_;
          pred_error_covar_ = MatrixXd::Zero(out_dim_, out_dim_);
        };

        void process_datum(const VectorXd& in_vec, const VectorXd& output);

        std::pair<MatrixXd, VectorXd> get_identified_mats() const;

        VectorXd predict(const VectorXd& in_vec);

        void reset();


      private:
        RowVectorXd make_feature_vec_(const VectorXd& in_vec) const;
        MatrixX2d make_u_(const RowVectorXd& feat) const;
        Matrix2Xd make_v_(const RowVectorXd& feat) const;
        Matrix2d make_c_() const;

        void update_covar_(const MatrixX2d& u, const Matrix2d& c, 
            const Matrix2Xd& v);
        void update_theta_(const MatrixXd& c_t, const RowVectorXd& feat, 
            const VectorXd& output);
        void update_output_mean_(const VectorXd& output);
        void update_feat_mean_(const RowVectorXd& feat);
        void update_pred_error_covar_(const VectorXd& in_vec, const VectorXd& output);

        // Parameters
        unsigned int in_dim_;
        unsigned int out_dim_;
        unsigned int param_dim_;
        double alpha_;
        double forgetting_;
        double t_ = 0.0;

        // Matrices
        VectorXd intercept_;
        MatrixXd theta_;
        MatrixXd corrected_theta_;
        RowVectorXd feat_mean_;
        RowVectorXd output_mean_;
        MatrixXd covar_;
        MatrixXd pred_error_covar_;

    };

  } // namespace ml
} // namespace cannon

#endif /* ifndef CANNON_ML_RLS_H */
