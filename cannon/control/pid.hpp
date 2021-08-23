#ifndef CANNON_CONTROL_PID_H
#define CANNON_CONTROL_PID_H 

/*!
 * \file cannon/control/pid.hpp
 * \brief File containing PidController class definition.
 */

#include <Eigen/Dense>

using namespace Eigen;

namespace cannon {
  namespace control {

    /*!
     * \brief Class representing a PID controller around a reference.
     */
    class PidController {
      public:

        /*!
         * \brief Constructor taking the state and control dimensionality of
         * the system to be controlled.
         */
        PidController(unsigned int state_dim = 1, unsigned int control_dim = 1);

        /*!
         * \brief Get PID control for the input state.
         *
         * \param state The state to compute PID control for. Note that this is
         * in the global frame, not the error frame.
         *
         * \returns The computed PID control.
         */
        VectorXd get_control(const Ref<const VectorXd>& state);

      private:
        unsigned int state_dim_; //!< State dimension of system to be controlled
        unsigned int control_dim_; //!< Control dimension of system to be controlled

        VectorXd ref_; //!< Current reference
        VectorXd last_error_state_; //!< Last observed error state, used for discrete derivative
        VectorXd integrated_error_; //!< Integrated error for integral portion

        MatrixXd Kp_; //!< Proportional term gain matrix
        MatrixXd Ki_; //!< Integral term gain matrix
        MatrixXd Kd_; //!< Derivative term gain matrix

    };

  }
}

#endif /* ifndef CANNON_CONTROL_PID_H */
