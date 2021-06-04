#ifndef CANNON_ML_ADAM_H
#define CANNON_ML_ADAM_H 

/*!
 * \file cannon/ml/adam.hpp
 * \brief File containing AdamOptimizer class definition.
 */

/*!
 * \namespace cannon::ml;
 * \brief Namespace containing various optimization and learning algorithms. 
 *
 * ML, or Machine Learning, is being used as an umbrella term here to suggest that
 * everything in this namespace has some relationship to optimizing function
 * approximations using data, but many of these methods are not, strictly
 * speaking, machine learning methods.
 */

#include <Eigen/Dense>

using namespace Eigen;

namespace cannon {
  namespace ml {

    /*!
     * \brief Class representing an Adam optimizer. Adam is a stochastic
     * optimization method which produces modified stochastic gradients by
     * estimating the moments of the distribution of stochastic gradients. See
     * https://arxiv.org/abs/1412.6980
     */
    class AdamOptimizer {
      public:
        AdamOptimizer() = delete;

        /*!
         * \brief Constructor taking the number of rows and columns of the
         * parameter matrix to be optimized, as well as a learning rate for
         * gradient-based updates.
         */
        AdamOptimizer(unsigned int rows, unsigned int cols,
                      double learning_rate)
            : cols_(cols), rows_(rows), learning_rate_(learning_rate),
              first_moments_(MatrixXd::Zero(rows_, cols_)),
              second_moments_(MatrixXd::Zero(rows_, cols_)) {}

        /*!
         * \brief Apply a stochastic gradient update by modifying the moment
         * estimates of this optimizer with the input gradient. 
         *
         * \param params Current parameter values
         * \param gradient Stochastic gradient
         * 
         * \returns New parameters, adjusted using Adam.
         */
        MatrixXd apply_update(const MatrixXd& params, const MatrixXd& gradient);

        /*!
         * \brief Get modified Adam update given an input stochastic gradient.
         * Also modifies the moment estimates of this optimizer.
         *
         * \param gradient Stochastic gradient.
         *
         * \returns Adam update
         */
        MatrixXd get_update(const MatrixXd& gradient);

        /*!
         * \brief Set the learning rate for this optimizer.
         *
         * \param learning_rate The new learning rate.
         */
        void set_learning_rate(double learning_rate);

      private:
        // Parameters
        unsigned int cols_; //!< Number of columns in parameter matrix
        unsigned int rows_; //!< Number of rows in parameter matrix
        double learning_rate_; //!< Learning rate for parameter updates

        // Hyperparameters
        double beta_1_ = 0.9; //!< Exponential averaging parameter for first-order moments
        double beta_2_ = 0.999; //!< Exponential averaging parameter for second-order moments
        double epsilon_ = 1e-8; //!< Small positive number to prevent division by zero

        unsigned int t_ = 0; //!< Number of stochastic gradients seen so far

        // Matrices
        MatrixXd first_moments_; //!< First-order moments of stochastic gradient distribution
        MatrixXd second_moments_; //!< Exponential averaging parameter for first-order moments
    };

  } // namespace ml
} // namespace cannon

#endif /* ifndef CANNON_ML_ADAM_H */
