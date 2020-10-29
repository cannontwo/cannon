#ifndef CANNON_CONTROL_AFFINE_CONTROLLER_H
#define CANNON_CONTROL_AFFINE_CONTROLLER_H 

#include <cassert>

#include <Eigen/Dense>

#include <cannon/ml/adam.hpp>

using namespace Eigen;
using namespace cannon::ml;

namespace cannon {
  namespace control {

    class AffineController {
      public:
        AffineController() = delete;

        AffineController(unsigned int state_dim, unsigned int action_dim,
            double learning_rate=1e-4, double decay=0.1, bool use_adam=true) :
        state_dim_(state_dim), action_dim_(action_dim),
        learning_rate_(learning_rate), initial_learning_rate_(learning_rate),
        use_adam_(use_adam), decay_(decay), K_opt_(action_dim, state_dim,learning_rate), 
        k_opt_(action_dim, 1, learning_rate) {
          K_ = MatrixXd::Zero(action_dim_, state_dim_);
          k_ = VectorXd::Zero(action_dim_);
        }

        VectorXd get_action(const VectorXd& state) const;

        void apply_gradient(const VectorXd& k_gradient, const MatrixXd& K_gradient);

      private:
        // Parameters
        unsigned int state_dim_;
        unsigned int action_dim_;
        double learning_rate_;
        double initial_learning_rate_;
        bool use_adam_;
        double decay_;

        unsigned int overall_t_ = 0;

        // Matrices
        MatrixXd K_;
        VectorXd k_;
        
        // Optimizers
        AdamOptimizer K_opt_;
        AdamOptimizer k_opt_;
    };

  } // namespace control
} // namespace cannon

#endif /* ifndef CANNON_CONTROL_AFFINE_CONTROLLER_H */
