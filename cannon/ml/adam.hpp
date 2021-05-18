#ifndef CANNON_ML_ADAM_H
#define CANNON_ML_ADAM_H 

#include <Eigen/Dense>

using namespace Eigen;

namespace cannon {
  namespace ml {

    class AdamOptimizer {
      public:
        AdamOptimizer() = delete;

        AdamOptimizer(unsigned int rows, unsigned int cols, double
            learning_rate) : cols_(cols), rows_(rows),
        learning_rate_(learning_rate) {
          first_moments_ = MatrixXd::Zero(rows_, cols_);
          second_moments_ = MatrixXd::Zero(rows_, cols_);
        }

        MatrixXd apply_update(const MatrixXd& params, const MatrixXd& gradient);

        void set_learning_rate(double learning_rate);

      private:
        // Parameters
        unsigned int cols_;
        unsigned int rows_;
        double learning_rate_;

        // Hyperparameters
        double beta_1_ = 0.9;
        double beta_2_ = 0.999;
        double epsilon_ = 1e-8;

        unsigned int t_ = 0;

        // Matrices
        MatrixXd first_moments_;
        MatrixXd second_moments_;
    };

  } // namespace ml
} // namespace cannon

#endif /* ifndef CANNON_ML_ADAM_H */
