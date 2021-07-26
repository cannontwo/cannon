#pragma once
#ifndef CANNON_PHYSICS_RK4_INTEGRATOR_H
#define CANNON_PHYSICS_RK4_INTEGRATOR_H 

/*!
 * \file cannon/physics/rk4_integrator.hpp
 * \brief File containing RK4Integrator class definition.
 */

#include <functional>

#include <Eigen/Dense>
#include <boost/numeric/odeint.hpp>
#include <boost/numeric/odeint/external/eigen/eigen_algebra.hpp>

using namespace Eigen;
using namespace boost::numeric::odeint;

namespace cannon {
  namespace physics {

    /*!
     * \brief Class representing a fourth-order Runge-Kutta integrator.
     */
    class RK4Integrator {
      public:
        using system_type = std::function<void(const VectorXd&, VectorXd&, double)>;

        RK4Integrator() = delete;

        /*!
         * \brief Constructor taking the system to integrate, state dimension,
         * and timestep for integration.
         */
        RK4Integrator(system_type system, unsigned int state_dim, double
            dt) : system_(system), state_dim_(state_dim), dt_(dt) {
          state_ = VectorXd::Zero(state_dim_); 
        }

        /*!
         * \brief Perform a single integration step using the timestep of this
         * integrator.
         *
         * \returns The new state, post-integration.
         */
        const VectorXd& step();

        /*!
         * \brief Set the state of the system being integrated.
         *
         * \param state The state to set.
         */
        void set_state(const VectorXd& state);

        /*!
         * \brief Get the current state of the system being integrated.
         *
         * \returns The current state.
         */
        VectorXd get_state() const;

        /*!
         * \brief Set the current time for the system being integrated.
         *
         * \param t The time to set.
         */
        void set_time(double t);

        /*!
         * \brief Get the current time in the system being integrated.
         *
         * \returns The current time.
         */
        double get_time() const;

      private:
        //using stepper_type = runge_kutta4<VectorXd, double, VectorXd, double, vector_space_algebra>;
        using stepper_type = runge_kutta_dopri5<VectorXd, double, VectorXd, double, vector_space_algebra>;

        system_type system_; //!< The system to be integrated
        stepper_type stepper_; //!< Internal ODEInt stepper

        unsigned int state_dim_; //!< State dimension of the system being integrated
        VectorXd state_; //!< Current state of the system being integrated

        double dt_; //!< Integration timestep
        double t_ = 0.0; //!< Current time in the integrated system

    };

  } // namespace physics
} // namespace cannon

#endif /* ifndef CANNON_PHYSICS_RK4_INTEGRATOR_H */
