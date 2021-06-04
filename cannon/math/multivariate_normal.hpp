#ifndef CANNON_MATH_MULTIVARIATE_NORMAL_H
#define CANNON_MATH_MULTIVARIATE_NORMAL_H 

/*!
 * \file cannon/math/multivariate_normal.hpp
 * \brief File containing MultivariateNormal class definition.
 */

#include <random>

#include <Eigen/Dense>

using namespace Eigen;

namespace cannon {
  namespace math {

    /*!
     * \brief Class representing a multivariate normal distribution of
     * arbitrary dimensionality.
     */
    class MultivariateNormal {
      public:
        MultivariateNormal() = delete;

        /*!
         * \brief Constructor taking a covariance matrix. The distribution is
         * assumed to be centered at zero.
         */
        MultivariateNormal(const MatrixXd& covar) :
          MultivariateNormal(VectorXd::Zero(covar.rows()), covar) {}

        /*!
         * \brief Constructor taking a mean and covariance matrix.
         */
        MultivariateNormal(const VectorXd &mean, const MatrixXd &covar)
            : mean_(mean),
              transform_(MatrixXd::Zero(mean.size(), mean.size())) {
          SelfAdjointEigenSolver<MatrixXd> eigen_solver(covar);
          transform_ = eigen_solver.eigenvectors() *
            eigen_solver.eigenvalues().cwiseSqrt().asDiagonal();
        }

        /*!
         * \brief Sample a point from this distribution.
         *
         * \returns A sample from the multivariate normal distribution
         * represented by this object.
         */
        VectorXd sample() const {
          return mean_ + (transform_ * VectorXd::NullaryExpr(mean_.size(),
                [&](){return dist(gen);}));
        }

        /*!
         * \brief Set the seed for all normal distributions.
         *
         * \param seed The new seed to use.
         */ 
        static void set_seed(int seed);

        static std::random_device rd; //!< Physical random number generator.
        static std::mt19937 gen; //!< Pseudo-random number generator.
        static std::normal_distribution<double> dist; //!< 1D normal distribution for sampling

      private:
        VectorXd mean_; //!< Mean of this normal distribution
        MatrixXd transform_; //!< Transform used to compute multivariate samples

    };

  }
}

#endif /* ifndef CANNON_MATH_MULTIVARIATE_NORMAL_H */
