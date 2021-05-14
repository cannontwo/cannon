#ifndef CANNON_MATH_MULTIVARIATE_NORMAL_H
#define CANNON_MATH_MULTIVARIATE_NORMAL_H 

#include <random>

#include <Eigen/Dense>

using namespace Eigen;

namespace cannon {
  namespace math {

    class MultivariateNormal {
      public:
        MultivariateNormal() = delete;

        MultivariateNormal(const MatrixXd& covar) :
          MultivariateNormal(VectorXd::Zero(covar.rows()), covar) {}

        MultivariateNormal(const VectorXd& mean, const MatrixXd& covar) : mean_(mean) {
          SelfAdjointEigenSolver<MatrixXd> eigen_solver(covar);
          transform_ = eigen_solver.eigenvectors() *
            eigen_solver.eigenvalues().cwiseSqrt().asDiagonal();
        }

        VectorXd sample() const {
          return mean_ + (transform_ * VectorXd::NullaryExpr(mean_.size(),
                [&](){return dist(gen);}));
        }

        static void set_seed(int seed);

        static std::random_device rd;
        static std::mt19937 gen;
        static std::normal_distribution<double> dist;

      private:
        VectorXd mean_;
        MatrixXd transform_;


    };

  }
}

#endif /* ifndef CANNON_MATH_MULTIVARIATE_NORMAL_H */
