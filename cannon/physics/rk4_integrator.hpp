#ifndef CANNON_PHYSICS_RK4_INTEGRATOR_H
#define CANNON_PHYSICS_RK4_INTEGRATOR_H 

#include <functional>

#include <Eigen/Dense>
#include <boost/numeric/odeint.hpp>
#include <boost/numeric/odeint/external/eigen/eigen_algebra.hpp>

using namespace Eigen;
using namespace boost::numeric::odeint;

namespace cannon {
  namespace physics {

    class RK4Integrator {
      public:
        using system_type = std::function<void(const VectorXd&, VectorXd&, double)>;

        RK4Integrator() = delete;

        RK4Integrator(system_type system, unsigned int state_dim, double
            dt) : system_(system), state_dim_(state_dim), dt_(dt) {
          state_ = VectorXd::Zero(state_dim_); 
        }

        VectorXd step();

        // Getters and setters
        void set_state(const VectorXd& state);
        VectorXd get_state() const;

        void set_time(double t);
        double get_time();

      private:
        //using stepper_type = runge_kutta4<VectorXd, double, VectorXd, double, vector_space_algebra>;
        using stepper_type = runge_kutta_dopri5<VectorXd, double, VectorXd, double, vector_space_algebra>;

        system_type system_;
        stepper_type stepper_;

        unsigned int state_dim_;
        VectorXd state_;

        double dt_;
        double t_ = 0.0;

    };

  } // namespace physics
} // namespace cannon

#endif /* ifndef CANNON_PHYSICS_RK4_INTEGRATOR_H */
