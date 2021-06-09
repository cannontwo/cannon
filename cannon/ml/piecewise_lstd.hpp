#ifndef CANNON_ML_PIECEWISE_LSTD_H
#define CANNON_ML_PIECEWISE_LSTD_H 

/*!
 * \file cannon/ml/piecewise_lstd.hpp
 * \brief File containing PiecewiseLSTDFilter class definition.
 */

#include <Eigen/Core>

using namespace Eigen;

namespace cannon {
  namespace ml {

    /*!
     * \brief Class representing a Least-Squares Temporal Difference (LSTD)
     * approximator using a feature set that produces a piecewise-affine value
     * function approximation. This is for use in reinforcement learning algorithms.
     */
    class PiecewiseLSTDFilter {
      public:
        PiecewiseLSTDFilter() = delete;

        /*!
         * \brief Constructor taking state space dimension, number of affine
         * regions, and discount factor.
         */
        PiecewiseLSTDFilter(unsigned int in_dim, unsigned int num_refs,
                            double discount_factor, double alpha = 1.0)
            : in_dim_(in_dim + 1), param_dim_(in_dim_ * num_refs),
              num_refs_(num_refs), discount_factor_(discount_factor),
              alpha_(alpha),
              a_inv_(MatrixXd::Identity(param_dim_, param_dim_) * alpha_),
              b_(VectorXd::Zero(param_dim_)),
              theta_(VectorXd::Zero(param_dim_)) {}

        /*!
         * \brief Update this approximation in light of a single data point,
         * which is a transition from one state to another with associated
         * reward.
         *
         * \param in_vec Feature vector for first state.
         * \param next_in_vec Feature vector for next state.
         * \param idx Region index for first state.
         * \param next_idx Region index for next state.
         * \param reward Reward associated with this state transition.
         */
        void process_datum(const VectorXd &in_vec, const VectorXd &next_in_vec,
                           unsigned int idx, unsigned int next_idx,
                           double reward);

        /*!
         * \brief Get the matrix representing the linear portion of the local,
         * affine approximation in the region with for the input index.
         *
         * \param idx Region index
         *
         * \returns Estimated local linear approximation parameter matrix.
         */
        VectorXd get_mat(unsigned int idx);

        /*!
         * \brief Predict the value of the input state using the estimated
         * piecewise-affine value function.
         *
         * \param in_vec Feature vector for the state.
         * \param idx Region index of the state.
         *
         * \returns Value function prediction.
         */
        double predict(const VectorXd& in_vec, unsigned int idx);

        /*!
         * \brief Reset this value function approximation.
         */
        void reset();

      private:
        /*!
         * \brief Make internal feature vector for the input state in the
         * region with the input index.
         *
         * \param in_vec Input features for state.
         * \param idx Region index for the state.
         *
         * \returns Internal feature representation leading to piecewise-affine
         * function.
         */
        RowVectorXd make_feature_vec_(VectorXd in_vec, unsigned int idx) const;

        /*!
         * \brief Update approximation of the linear portion of the LSTD filter
         * given a particular state transition.
         *
         * \param feat Internal feature vector of the first state.
         * \param next_feat Internal feature vector of the next state.
         */
        void update_a_inv_(const RowVectorXd& feat, const RowVectorXd& next_feat);

        /*!
         * \brief Update approximation of the vector portion of the LSTD filter.
         *
         * \param feat Internal feature vector of the first state.
         * \param next_feat Internal feature vector of the next state.
         */
        void update_b_(const RowVectorXd& feat, double reward);

        /*!
         * \brief Update parameter vector theta, if necessary. This lazy
         * evaluation can save time if multiple value function updates occur
         * between predictions.
         */
        void check_theta_();

        // Parameters
        unsigned int in_dim_; //!< Dimension of input
        unsigned int param_dim_; //!< Dimension of internal feature space
        unsigned int num_refs_; //!< Number of linear regions
        double discount_factor_; //!< Discount factor for value function
        double alpha_; //!< L2 regularization parameter

        // Matrices
        MatrixXd a_inv_; //!< Linear portion of LSTD filter
        VectorXd b_; //!< Vector portion of LSTD filter
        VectorXd theta_; //!< Parameters of value function approximation
        bool theta_updated_ = false; //!< Whether theta has been updated since last A, b update
    };
    
  } // namespace ml
} // namespace cannon
#endif /* ifndef CANNON_ML_PIECEWISE_LSTD_H */
