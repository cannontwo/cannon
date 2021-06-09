#ifndef CANNON_PHYSICS_SYSTEMS_INVERTED_PENDULUM_H
#define CANNON_PHYSICS_SYSTEMS_INVERTED_PENDULUM_H 

#include <Eigen/Dense>

#include <ompl/control/ODESolver.h>
#include <ompl/control/spaces/RealVectorControlSpace.h>

#include <cannon/physics/systems/system.hpp>
#include <cannon/physics/rk4_integrator.hpp>

namespace oc = ompl::control;
namespace ob = ompl::base;

using namespace Eigen;

namespace cannon {
  namespace physics {
    namespace systems {

      struct Hol2DSystem : System {
        Hol2DSystem() {}

        virtual void operator()(const VectorXd& s, VectorXd& dsdt, const double /*t*/) override {
          double ux = s[2];
          double uy = s[3];

          dsdt.resize(4);
          dsdt[0] = ux;
          dsdt[1] = uy;
          dsdt[2] = 0.0;
          dsdt[3] = 0.0;
        }

        virtual void ompl_ode_adaptor(const oc::ODESolver::StateType& q, 
            const oc::Control* control, oc::ODESolver::StateType& qdot) override {

          const double ux = control->as<oc::RealVectorControlSpace::ControlType>()->values[0];
          const double uy = control->as<oc::RealVectorControlSpace::ControlType>()->values[1];

          VectorXd s(4);
          s[0] = q[0];
          s[1] = q[1];
          s[2] = ux;
          s[3] = uy;
          VectorXd dsdt(4);

          (*this)(s, dsdt, 0.0);

          qdot.resize(q.size(), 0);
          for (unsigned int i = 0; i < q.size(); i++) {
            qdot[i] = dsdt[i];
          }
        }

        virtual std::tuple<MatrixXd, MatrixXd, VectorXd> get_linearization(const VectorXd& x) override {
          MatrixXd A = MatrixXd::Zero(2, 2);
          MatrixXd B = MatrixXd::Zero(2, 2);
          VectorXd c = VectorXd::Zero(2);

          // TODO 
          throw std::runtime_error("Not implemented yet");
          
          return std::make_tuple(A, B, c);
        }

        virtual void
        get_continuous_time_linearization(const oc::ODESolver::StateType &q,
                                          Ref<MatrixXd> A,
                                          Ref<MatrixXd> B) override {
          // TODO
          throw std::runtime_error("Not implemented yet");
        }

        static void ompl_post_integration(const ob::State* /*state*/, const
            oc::Control* /*control*/, const double /*duration*/, ob::State *result) {
          // Nothing necessary
        }

        // Parameters?
        
      };

      class Holonomic2D {
        public:
          Holonomic2D() = delete;

          Holonomic2D(Vector2d s, Vector2d g) : e_(s_, 4, 0.01), start_(s), goal_(g) {
            state_ = Vector4d::Zero(); 
            reset();
          }

          std::pair<VectorXd, double> step(double ux, double uy) {
            double clipped_ux = std::max(-1.0, std::min(ux, 1.0));
            double clipped_uy = std::max(-1.0, std::min(uy, 1.0));

            state_[2] = clipped_ux;
            state_[3] = clipped_uy;

            double goal_r = -(state_.head(2) - goal_).norm();
            double control_r = -(std::abs(clipped_ux), + std::abs(clipped_uy));

            double reward = goal_r + 0.1*control_r;

            e_.set_state(state_);
            state_ = e_.step();

            return std::make_pair(state_.head(2), reward);
          }

          VectorXd reset() {
            state_.head(2) = start_ + Vector2d::Random() * 0.1;
            state_[2] = 0.0;
            state_[3] = 0.0;

            return state_.head(2);
          }

          Hol2DSystem s_;

        private:
          RK4Integrator e_;

          Vector4d state_;

          Vector2d start_;
          Vector2d goal_;

      };

    } // namespace systems
  } // namespace physics
} // namespace cannon

#endif /* ifndef CANNON_PHYSICS_SYSTEMS_INVERTED_PENDULUM_H */
