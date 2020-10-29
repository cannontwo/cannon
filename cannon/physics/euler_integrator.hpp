#ifndef CANNON_PHYSICS_EULER_INTEGRATOR_H
#define CANNON_PHYSICS_EULER_INTEGRATOR_H

#include <Eigen/Dense>
#include <boost/numeric/odeint.hpp>
#include <boost/numeric/odeint/external/eigen/eigen_algebra.hpp>

using namespace Eigen;
using namespace boost::numeric::odeint;

namespace cannon {
  namespace physics {

    template <typename F>
    class EulerIntegrator {
      public:
        EulerIntegrator() = delete;

        EulerIntegrator(F system, unsigned int state_dim, double dt) :
            system_(system), state_dim_(state_dim), dt_(dt) {
          state_ = VectorXd::Zero(state_dim_);
        }

        VectorXd step() {
          stepper_.do_step(system_, state_, t_, dt_);
          t_ += dt_;

          return state_;
        }

        // Getters and setters
        void set_state(const VectorXd& state) {
          state_ = state;
        }

        VectorXd get_state() const {
          return state_;
        }

        void set_time(double t) {
          t_ = t;
        }

        double get_time() {
          return t_;
        }

      private:
        using stepper_type = euler<VectorXd, double, VectorXd, double, vector_space_algebra>;
        // System should take a state, a vector in which to place its gradient, and a time
        F system_;
        stepper_type stepper_;

        unsigned int state_dim_;
        VectorXd state_;

        double dt_;
        double t_ = 0.0;
    };

    template <typename F>
    EulerIntegrator<F> make_euler_integrator(F system, unsigned int state_dim, double dt) {
      return EulerIntegrator<F>(system, state_dim, dt);
    }
    
  } // namespace physics
} // namespace cannon

#endif /* ifndef CANNON_PHYSICS_EULER_INTEGRATOR_H */
