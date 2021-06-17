#ifndef CANNON_CONTROL_LQR_H
#define CANNON_CONTROL_LQR_H 

/*!
 * \file cannon/control/lqr.hpp
 * \brief File containing utility functions for computing LQR controllers in
 * discrete and continuous time.
 */

#include <Eigen/Dense>

using namespace Eigen;

namespace cannon {
  namespace control {

    /*!
     * \brief Class representing an LQR controller for a continuous or
     * discrete-time system.
     */
    class LQRController {
      public:

        using LinearizationFunction =
            std::function<void(const Ref<const VectorXd> &, Ref<MatrixXd>,
                               Ref<MatrixXd>)>;

        LQRController() = delete;

        /*!
         * \brief Constructor taking state to stabilize around, control
         * dimension, whether the system to be controlled is continuous-time,
         * and linearization function for the system to be controlled.
         *
         * Initializes cost matrices to identity.
         */
        LQRController(const Ref<const VectorXd> &q0,
                      unsigned int control_dim,
                      LinearizationFunction f,
                      bool continuous = true);

        /*!
         * \brief Constructor taking state to stabilize around, state
         * cost matrix, control cost matrix, whether the system to be
         * controlled is continuous-time, and linearization function for the
         * system to be controlled.
         */
        LQRController(const Ref<const VectorXd> &q0,
                      const Ref<const MatrixXd> &Q,
                      const Ref<const MatrixXd> &R, LinearizationFunction f,
                      bool continuous = true);

        /*!
         * \brief Compute the control to apply at the input state.
         *
         * \param q The state to compute control for.
         * 
         * \returns The computed control.
         */
        VectorXd compute_control(const Ref<const VectorXd>& q);

        /*!
         * \brief Get the linear portion of the control law represented by this
         * controller.
         *
         * \returns Gain matrix.
         */
        MatrixXd get_linear_gain();

        /*!
         * \brief Get the constant offset portion of the control law
         * represented by this controller.
         *
         * \returns Offset vector.
         */
        VectorXd get_control_offset();

        /*!
         * \brief Set the state that this controller attempts to stabilize to.
         *
         * \param q The new state to stabilize to.
         */
        void set_target(const Ref<const VectorXd>& q);

      private:

        /*!
         * \brief Compute LQR gain for controller represented by this object.
         */
        void compute_lqr_gain_();

        VectorXd q0_; //!< State to stabilize toward
        MatrixXd Q_; //!< State cost matrix
        MatrixXd K_; //!< Control gain matrix
        MatrixXd R_; //!< Control cost matrix
        LLT<MatrixXd> R_cholesky_; //!< Cholesky factorization of control cost matrix
        LinearizationFunction linearization_; //!< Linearization function for system to be controlled
        bool continuous_; //!< Whether the system to be controlled is continuous-time
    };

    // Free Functions
    /*!
     * \brief Solve the continuous-time algebraic riccati equation for
     * the input state and control derivative matrices A and B, and
     * return the resulting LQR gain.
     *
     * \param A Partial derivatives of ode with respect to state.
     * \param B Partial derivatives of ode with respect to control.
     * \param Q State cost matrix
     * \param R Control cost matrix Cholesky decomposition
     *
     * \returns CARE solution control gain matrix.
     */
  MatrixXd continuous_algebraic_riccati_equation(const Ref<const MatrixXd> &A,
                                                 const Ref<const MatrixXd> &B,
                                                 const Ref<const MatrixXd> &Q,
                                                 const LLT<MatrixXd> &R_cholesky);

  } // namespace control
} // namespace cannon


#endif /* ifndef CANNON_CONTROL_LQR_H */
