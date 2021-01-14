#ifndef CANNON_CONTROL_AFFINE_CONTROLLER_H
#define CANNON_CONTROL_AFFINE_CONTROLLER_H 

/*!
 * \file cannon/control/affine_controller.hpp
 * File containing AffineController class definition.
 */

/*!
 * \namespace cannon::control
 * Namespace containing control-relevant classes and functions.
 */

#include <cassert>
#include <utility>

#include <Eigen/Dense>

#include <cannon/ml/adam.hpp>
#include <cannon/log/registry.hpp>

using namespace Eigen;

using namespace cannon::ml;
using namespace cannon::log;

namespace cannon {
  namespace control {

    /*!
     * \brief Class representing an affine-controller.
     *
     * An affine controller is one with a term linear in the state and a
     * constant offset; this controller is explicitly multidimensional and
     * designed to be updated by gradient descent.
     *
     * \sa cannon::ml::AdamOptimizer
     */
    class AffineController {
      public:
        AffineController() = delete;

        /*!
         * Constructor taking the state dimension and action (control) dimension.
         *
         * \param state_dim Dimensionality of state space
         * \param action_dim Dimensionality of action (control) space
         * \param learning_rate Learning rate for gradient-based updating of the controller
         * \param use_adam Whether to use the Adam optimizer. If false,
         * stochastic gradient descent is used
         *
         */
        AffineController(unsigned int state_dim, unsigned int action_dim,
            double learning_rate=1e-4, bool use_adam=true) :
        state_dim_(state_dim), action_dim_(action_dim),
        learning_rate_(learning_rate), initial_learning_rate_(learning_rate),
        use_adam_(use_adam), K_opt_(action_dim, state_dim,learning_rate), 
        k_opt_(action_dim, 1, learning_rate) {
          K_ = MatrixXd::Zero(action_dim_, state_dim_);
          k_ = VectorXd::Zero(action_dim_);
        }

        /*!
         * Computes the action (control) applied by this controller for the input state.
         *
         * \param state The state to compute an action for
         *
         * \returns The action applied by this controller
         */
        VectorXd get_action(const VectorXd& state) const;

        /*!
         * Returns the vector offset and linear terms defining this controller.
         * These are the k and K vector and matrix, respectively, in control
         * literature.
         *
         * \returns k and K, the constant and linear terms for this controller.
         */
        std::pair<VectorXd, MatrixXd> get_mats() const;

        /*!
         * Applies the input gradient to this controller, updating the k and K parameters.
         *
         * \param k_gradient The gradient to apply to the vector offset for this controller.
         * \param K_gradient The gradient to apply to the linear term of this controller.
         */
        void apply_gradient(const VectorXd& k_gradient, const MatrixXd& K_gradient);

      private:
        // Parameters
        unsigned int state_dim_; //!< Dimensionality of state space
        unsigned int action_dim_; //!< Dimensionality of action space
        double learning_rate_; //!< Current learning rate for updates
        double initial_learning_rate_; //!< Initial learning rate
        bool use_adam_; //!< Whether to use AdamOptimizer for updating

        /*!
         * \rst
         * .. todo:: Make learning rate decay parameter for AffineController
         * \endrst
         */
        double decay_ = 0.1; //!< Learning rate decay 

        unsigned int overall_t_ = 0; //!< Number of updates applied to this controller

        // Matrices
        MatrixXd K_; //!< Linear term for this controller
        VectorXd k_; //!< Constant offset for this controller
        
        // Optimizers
        AdamOptimizer K_opt_; //!< Optimizer for the K matrix of this controller
        AdamOptimizer k_opt_; //!< Optimizer for the k matrix of this controller
    };

  } // namespace control
} // namespace cannon

#endif /* ifndef CANNON_CONTROL_AFFINE_CONTROLLER_H */
