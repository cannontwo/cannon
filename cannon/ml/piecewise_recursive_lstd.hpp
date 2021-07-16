#ifndef CANNON_ML_RECURSIVE_PIECEWISE_LSTD_H
#define CANNON_ML_RECURSIVE_PIECEWISE_LSTD_H 

/*!
 * \file cannon/ml/piecewise_recursive_lstd.hpp
 * \brief File containing PiecewiseRecursiveLSTDFilter class definition.
 */

#include <Eigen/Core>
#include <Eigen/Sparse>

using namespace Eigen;

namespace cannon {
  namespace ml {

    /*!
     * \brief Class representing a Least-Squares Temporal Difference (LSTD)
     * approximator using a feature set that produces a piecewise-affine value
     * function approximation. This is for use in reinforcement learning
     * algorithms. This version of the algorithm is similar to a recursive
     * least squares filter.
     */
    class PiecewiseRecursiveLSTDFilter {
      public:
        PiecewiseRecursiveLSTDFilter() = delete;

        /*!
         * \brief Constructor taking state space dimension, number of affine
         * regions, and discount factor.
         */
        PiecewiseRecursiveLSTDFilter(unsigned int in_dim, unsigned int num_refs, double
            discount_factor, double alpha=1.0) : in_dim_(in_dim + 1),
        param_dim_(in_dim_ * num_refs), num_refs_(num_refs),
        discount_factor_(discount_factor), alpha_(alpha) {
          a_inv_ = MatrixXd::Identity(param_dim_, param_dim_) * alpha_;
          theta_ = VectorXd::Zero(param_dim_);
        }

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
        void process_datum(const VectorXd& in_vec, const VectorXd& next_in_vec,
            unsigned int idx, unsigned int next_idx, double reward);

        /*!
         * \brief Get the matrix representing the linear portion of the local,
         * affine approximation in the region with for the input index.
         *
         * \param idx Region index
         *
         * \returns Estimated local linear approximation parameter matrix.
         */
        VectorXd get_mat(unsigned int idx) const;

        /*!
         * \brief Predict the value of the input state using the estimated
         * piecewise-affine value function.
         *
         * \param in_vec Feature vector for the state.
         * \param idx Region index of the state.
         *
         * \returns Value function prediction.
         */
        double predict(const VectorXd& in_vec, unsigned int idx) const;

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
        SparseMatrix<double>
        make_feature_vec_(VectorXd in_vec, unsigned int idx) const;

        /*!
         * \brief Update approximation of the linear portion of the LSTD filter
         * given a particular state transition.
         *
         * \param feat Internal feature vector of the first state.
         * \param next_feat Internal feature vector of the next state.
         */
        void update_a_inv_(const SparseMatrix<double> &feat,
                           const SparseMatrix<double> &next_feat);

        // Parameters
        unsigned int in_dim_; //!< Dimension of input
        unsigned int param_dim_; //!< Dimension of internal feature space
        unsigned int num_refs_; //!< Number of linear regions
        double discount_factor_; //!< Discount factor for value function
        double alpha_; //!< L2 regularization parameter

        // Matrices
        MatrixXd a_inv_; //!< Linear portion of LSTD filter
        VectorXd theta_; //!< Parameters of value function approximation
    };
    
  } // namespace ml
} // namespace cannon
#endif /* ifndef CANNON_ML_RECURSIVE_PIECEWISE_LSTD_H */
