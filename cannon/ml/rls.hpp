#ifndef CANNON_ML_RLS_H
#define CANNON_ML_RLS_H 

/*!
 * \file cannon/ml/rls.hpp
 * \brief File containing RLSFilter class definition.
 */

#include <utility>

#include <Eigen/Dense>

using namespace Eigen;

namespace cannon {
  namespace ml {

    /*!
     * \brief Class representing a Recursive Least Squares Filter supporting
     * intercept estimation and forgetting. See
     * http://cannontwo.com/assets/rls_notes.pdf.
     */
    class RLSFilter {
      public:

        RLSFilter() = delete;

        /*!
         * \brief Constructor taking input dimension, output dimension, inverse
         * covariance matrix initialization alpha, and forgetting factor.
         */
        RLSFilter(unsigned int in_dim, unsigned int out_dim,
                  double alpha = 1.0e6, double forgetting = 1.0)
            : in_dim_(in_dim), out_dim_(out_dim), param_dim_(in_dim),
              alpha_(alpha), forgetting_(forgetting),
              intercept_(VectorXd::Zero(out_dim_)),
              theta_(MatrixXd::Zero(param_dim_, out_dim_)),
              corrected_theta_(MatrixXd::Zero(param_dim_, out_dim_)),
              feat_mean_(RowVectorXd::Zero(param_dim_)),
              output_mean_(RowVectorXd::Zero(out_dim_)),
              covar_(MatrixXd::Identity(param_dim_, param_dim_) * alpha_),
              pred_error_covar_(MatrixXd::Zero(out_dim_, out_dim_)) {}

        /*!
         * \brief Get number of data points contributing to this filter.
         *
         * \returns Number of data points used to fit this filter.
         */
        unsigned int get_num_data() const;

        /*!
         * \brief Process a single datum and update this filter.
         *
         * \param in_vec Input features of the datum to process.
         * \param output Output features of the datum to process.
         */
        void process_datum(const VectorXd& in_vec, const VectorXd& output);

        /*!
         * \brief Get the identified linear parameters and offset vector fit by
         * this filter.
         *
         * \returns A pair containing the linear parameters and offset vector.
         */ 
        std::pair<MatrixXd, VectorXd> get_identified_mats() const;
        
        /*!
         * \brief Get prediction error covariance matrix for this filter.
         *
         * \returns Prediction error covariance matrix.
         */
        MatrixXd get_pred_error_covar() const;

        /*!
         * \brief Predict the value of the linear approximation fit by this
         * filter for the input point.
         *
         * \param in_vec Input features of point to predict for.
         *
         * \returns Prediction fit by this filter.
         */
        VectorXd predict(const VectorXd& in_vec) const;

        /*!
         * \brief Set the parameters of this RLS filter. Input and output mean
         * are recovered from the intercept.
         *
         * \param theta Parameter matrix to set.
         * \param intercept Intercept to set.
         * \param in_mean Input mean. 
         */
        void set_params(const Ref<const MatrixXd> &theta,
                        const Ref<const VectorXd> &intercept,
                        const Ref<const VectorXd> &in_mean);

        /*!
         * \brief Reset this filter.
         */
        void reset();

      private:

        /*!
         * \brief Make internal feature vector for the input features.
         *
         * \param in_vec Input features to transform into internal feature space.
         *
         * \returns Internal features.
         */
        RowVectorXd make_feature_vec_(const VectorXd& in_vec) const;
        
        /*!
         * \brief Make U matrix for inverse covariance matrix update.
         *
         * \param feat Internal feature vector for update.
         *
         * \returns U matrix.
         */
        MatrixX2d make_u_(const RowVectorXd& feat) const;

        /*!
         * \brief Make V matrix for inverse covariance matrix update.
         *
         * \param feat Internal feature vector for update.
         *
         * \returns V matrix.
         */
        Matrix2Xd make_v_(const RowVectorXd& feat) const;

        /*!
         * \brief Make C matrix for inverse covariance matrix update.
         *
         * \returns C matrix.
         */
        Matrix2d make_c_() const;

        /*!
         * \brief Update inverse covariance matrix for this filter.
         *
         * \param u U matrix
         * \param c C matrix
         * \param v V matrix
         */
        void update_covar_(const MatrixX2d& u, const Matrix2d& c, 
            const Matrix2Xd& v);

        /*!
         * \brief Update linear approximation stored by this filter.
         *
         * \param c_t C matrix transpose.
         * \param feat Internal feature vector for new datum.
         * \param output Output for new datum.
         */
        void update_theta_(const MatrixXd& c_t, const RowVectorXd& feat, 
            const VectorXd& output);
        
        /*!
         * \brief Update output mean for this filter.
         *
         * \param output Observed output.
         */
        void update_output_mean_(const VectorXd& output);

        /*!
         * \brief Update internal feature mean for this filter.
         *
         * \param feat Observed internal features.
         */
        void update_feat_mean_(const RowVectorXd& feat);

        /*!
         * \brief Update prediction error covariance matrix in light of an
         * observed datum.
         *
         * \param in_vec Input feature vector.
         * \param output Observed output.
         */
        void update_pred_error_covar_(const VectorXd& in_vec, const VectorXd& output);

        // Parameters
        unsigned int in_dim_; //!< Dimension of input to this filter.
        unsigned int out_dim_; //!< Dimension of output for this filter.
        unsigned int param_dim_; //!< Number of linear parameters of this filter.
        double alpha_; //!< Inverse covariance matrix initialization parameter.
        double forgetting_; //!< Forgetting factor.
        double t_ = 0.0; //!< Number of observed data points.

        // Matrices
        VectorXd intercept_; //!< Intercept vector of filter
        MatrixXd theta_; //!< Linear parameters of filter
        MatrixXd corrected_theta_; //!< Linear parameters corrected for intercept
        RowVectorXd feat_mean_; //!< Mean of internal features for this filter
        RowVectorXd output_mean_; //!< Mean of observed outputs for this filter
        MatrixXd covar_; //!< Internal inverse covariance matrix.
        MatrixXd pred_error_covar_; //!< Prediction error covariance matrix for this filter.

    };

  } // namespace ml
} // namespace cannon

#endif /* ifndef CANNON_ML_RLS_H */
