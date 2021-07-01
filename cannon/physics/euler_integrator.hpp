#ifndef CANNON_PHYSICS_EULER_INTEGRATOR_H
#define CANNON_PHYSICS_EULER_INTEGRATOR_H

/*!
 * \file cannon/physics/euler_integrator.hpp
 * \brief File containing EulerIntegrator class definition.
 */

#include <Eigen/Dense>
#include <boost/numeric/odeint.hpp>
#include <boost/numeric/odeint/external/eigen/eigen_algebra.hpp>

using namespace Eigen;
using namespace boost::numeric::odeint;

namespace cannon {
  namespace physics {

    // TODO Refactor to use std::function
    /*!
     * \brief Class representing an Euler Integrator. Essentially just wraps
     * boost::odeint functionality.
     */
    template <typename F>
    class EulerIntegrator {
      public:
        EulerIntegrator() = delete;

        /*!
         * \brief Constructor taking an ODE, the state dimension, and the
         * integration timestep.
         */
        EulerIntegrator(F system, unsigned int state_dim, double dt) :
            system_(system), state_dim_(state_dim), dt_(dt) {
          state_ = VectorXd::Zero(state_dim_);
        }

        /*!
         * \brief Step this integrator by a single timestep.
         *
         * \returns The new state, post-step.
         */
        VectorXd step() {
          stepper_.do_step(system_, state_, t_, dt_);
          t_ += dt_;

          return state_;
        }

        /*!
         * \brief Set the state of this integrator.
         *
         * \param state The new state.
         */
        void set_state(const VectorXd& state) {
          state_ = state;
        }

        /*!
         * \brief Get the current state of this integrator.
         *
         * \returns Current state of this integrator.
         */
        VectorXd get_state() const {
          return state_;
        }

        /*!
         * \brief Set the current time for this integrator.
         *
         * \param t The new time.
         */
        void set_time(double t) {
          t_ = t;
        }

        /*!
         * \brief Get the current time for this integrator.
         *
         * \returns The current time.
         */
        double get_time() {
          return t_;
        }

      private:
        using stepper_type = euler<VectorXd, double, VectorXd, double, vector_space_algebra>; 
        // System should take a state, a vector in which to place its gradient, and a time
        F system_; //!< ODE to be integrated. Should take a state, gradient vector, and time
        stepper_type stepper_; //!< Stepper for integration

        unsigned int state_dim_; //!< Dimension of state vector.
        VectorXd state_; //!< Current state of system

        double dt_; //!< System integration timestep
        double t_ = 0.0; //!< Overall time
    };

    /*!
     * \brief Make an EulerIntegrator, inferring the function pointer type
     * automatically.
     *
     * \param system The system ODE to integrate. 
     * \param state_dim Dimensionality of real vector state.
     * \param dt Time step for integration.
     *
     * \returns Constructed EulerIntegrator.
     */
    template <typename F>
    EulerIntegrator<F> make_euler_integrator(F system, unsigned int state_dim, double dt) {
      return EulerIntegrator<F>(system, state_dim, dt);
    }
    
  } // namespace physics
} // namespace cannon

#endif /* ifndef CANNON_PHYSICS_EULER_INTEGRATOR_H */
