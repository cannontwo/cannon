#ifndef CANNON_CONTROL_PID_H
#define CANNON_CONTROL_PID_H 

/*!
 * \file cannon/control/pid.hpp
 * \brief File containing PidController class definition.
 */

#include <Eigen/Dense>

#include <cannon/utils/class_forward.hpp>

using namespace Eigen;

namespace cannon {

  namespace geom {
    CANNON_CLASS_FORWARD(Trajectory);
    CANNON_CLASS_FORWARD(ControlledTrajectory);  
  }

  using namespace cannon::geom;

  namespace control {

    /*!
     * \brief Class representing a PID controller around a reference.
     */
    class PidController {
      public:

        /*!
         * \brief Constructor taking the state and control dimensionality of
         * the system to be controlled, as well as sampling time for
         * digital control.
         */
        PidController(unsigned int state_dim = 1, unsigned int control_dim = 1,
                      double timestep = 0.01);

        /*!
         * \brief Get PID control for the input state. Also affects integral
         * and derivative estimation.
         *
         * \param state The state to compute PID control for. Note that this is
         * in the global frame, not the error frame.
         *
         * \returns The computed PID control.
         */
        VectorXd get_control(const Ref<const VectorXd>& state);

        /*!
         * \brief Reset the internal derivative and integral estimation of this
         * PID controller.
         */
        void reset();

        /*!
         * \brief Get a modifiable reference to the reference set for this PID
         * controller. The dimension of this vector should not be changed.
         *
         * \returns Reference to the reference vector.
         */
        Ref<VectorXd> ref() {
          return ref_;
        }

        /*!
         * \brief Get a modifiable reference to the proportional gain matrix
         * for this PID controller. Matrix dimensions shouldn't be changed.
         *
         * \returns Modifiable reference to proportional gain matrix.
         */
        Ref<MatrixXd> proportional_gain() {
          return Kp_;
        }

        /*!
         * \brief Get a modifiable reference to the integral gain matrix
         * for this PID controller. Matrix dimensions shouldn't be changed.
         *
         * \returns Modifiable reference to integral gain matrix.
         */
        Ref<MatrixXd> integral_gain() {
          return Ki_;
        }

        /*!
         * \brief Get a modifiable reference to the derivative gain matrix
         * for this PID controller. Matrix dimensions shouldn't be changed.
         *
         * \returns Modifiable reference to derivative gain matrix.
         */
        Ref<MatrixXd> derivative_gain() {
          return Kd_;
        }

      private:
        unsigned int state_dim_; //!< State dimension of system to be controlled
        unsigned int control_dim_; //!< Control dimension of system to be controlled
        double time_step_; //!< Control sampling timestep

        VectorXd ref_; //!< Current reference
        VectorXd penultimate_error_state_; //!< Second-to-last observed error state
        VectorXd last_error_state_; //!< Last observed error state, used for discrete derivative
        VectorXd last_control_; //!< Previous computed control

        MatrixXd Kp_; //!< Proportional term gain matrix
        MatrixXd Ki_; //!< Integral term gain matrix
        MatrixXd Kd_; //!< Derivative term gain matrix

    };

    /*!
     * \brief Construct a controlled trajectory tracking the input geometric
     * path using the input PID controller and the input model.
     *
     * \param system Dynamic system simulator to compute controlled trajectory in. Note that there are no checks on the dimensions of the arguments to this system, so watch for dimension errors.
     * \param traj Geometric trajectory to track.
     * \param controller PID controller to track trajectory with.
     * \param controlled_dims Number of state dimensions to feed to PID controller.
     * \param total_dims Total number of state dimensions
     * \param timestep Integration timestep of the input system.
     *
     * \returns The generated controlled trajectory in the input simulator.
     */
    ControlledTrajectory get_pid_controlled_trajectory(
        std::function<VectorXd(const Ref<const VectorXd> &,
                               const Ref<const VectorXd> &)>
            system,
        const Trajectory &traj, PidController &controller,
        unsigned int controlled_dims, unsigned int total_dims,
        double timestep = 0.01);
  }
}

#endif /* ifndef CANNON_CONTROL_PID_H */
