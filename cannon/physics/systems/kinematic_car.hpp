#ifndef CANNON_PHYSICS_SYSTEMS_KINEMATIC_CAR_H
#define CANNON_PHYSICS_SYSTEMS_KINEMATIC_CAR_H 

#include <random>

#include <ompl/control/ODESolver.h>
#include <ompl/control/spaces/RealVectorControlSpace.h>
#include <ompl/base/spaces/SO2StateSpace.h>
#include <ompl/base/spaces/SE2StateSpace.h>

#include <Eigen/Dense>

#include <cannon/physics/rk4_integrator.hpp>
#include <cannon/physics/systems/system.hpp>
#include <cannon/log/registry.hpp>

using namespace Eigen;

using namespace cannon::log;

namespace oc = ompl::control;
namespace ob = ompl::base;

namespace cannon {
  namespace physics {
    namespace systems {

      struct KinCarSystem : System {
        KinCarSystem(double l = 1.0) : l(l) {}

        virtual void operator()(const VectorXd& s, VectorXd& dsdt, const double /*t*/) override {
          double th = s[2];
          double uv = s[3];
          double uth = s[4];

          dsdt.resize(5);
          dsdt[0] = uv * std::cos(th);
          dsdt[1] = uv * std::sin(th);
          dsdt[2] = (uv / l) * std::tan(uth);
          dsdt[3] = 0.0;
          dsdt[4] = 0.0;
        }

        virtual void ompl_ode_adaptor(const oc::ODESolver::StateType& q, 
            const oc::Control* control, oc::ODESolver::StateType& qdot) override {

          const double uv = control->as<oc::RealVectorControlSpace::ControlType>()->values[0];
          const double uth = control->as<oc::RealVectorControlSpace::ControlType>()->values[1];

          VectorXd s(5);
          s[0] = q[0];
          s[1] = q[1];
          s[2] = q[2];
          s[3] = uv;
          s[4] = uth;
          VectorXd dsdt(5);

          (*this)(s, dsdt, 0.0);

          qdot.resize(q.size(), 0);
          for (unsigned int i = 0; i < q.size(); i++) {
            qdot[i] = dsdt[i];
          }
        }

        static void ompl_post_integration(const ob::State* /*state*/, const
            oc::Control* /*control*/, const double /*duration*/, ob::State *result) {

          ob::SO2StateSpace SO2;
          SO2.enforceBounds(result->as<ob::SE2StateSpace::StateType>()->as<ob::SO2StateSpace::StateType>(1));
        }

        // Parameters
        double l;
      };

      class KinematicCar {
        public:
          KinematicCar() = delete;

          KinematicCar(Vector3d s, Vector3d g) : e_(s_, 4, time_step), start_(s), goal_(g) {
            std::random_device rd;
            gen_ = std::mt19937(rd());  

            xy_dis_ = std::uniform_real_distribution<double>(-1.0, 1.0);
            th_dis_ = std::uniform_real_distribution<double>(-M_PI, M_PI);

            state_ = VectorXd::Zero(5);
            reset();
          }

          std::pair<VectorXd, double> step(double uv, double uth) {
            double clipped_uv = std::max(-1.0, std::min(uv, 1.0));
            //double clipped_uth = std::max(-M_PI, std::min(uth, M_PI));
            
            state_[3] = clipped_uv;
            state_[4] = uth;

            double goal_r = -std::pow((state_.head(2) - goal_.head(2)).norm(), 2.0);
            double control_r = -std::pow((std::abs(clipped_uv) + std::abs(uth)), 2.0);
            double reward = goal_r + 0.001*control_r;

            e_.set_state(state_);
            state_ = e_.step();

            return std::make_pair(state_.head(3), reward);
          }
          
          VectorXd reset() {
            //state_.head(3) = start_ + Vector3d::Random() * 0.1;
            state_[0] = xy_dis_(gen_);
            state_[1] = xy_dis_(gen_);
            state_[2] = th_dis_(gen_);
            
            state_[3] = 0.0;
            state_[4] = 0.0;

            return state_.head(3);
          }

          VectorXd reset(const VectorXd& s) {
            state_[0] = s[0];
            state_[1] = s[1];
            state_[2] = s[2];
            
            state_[3] = 0.0;
            state_[4] = 0.0;

            return state_.head(3);

          }

          // In seconds
          const double time_step = 0.01;

          KinCarSystem s_;
          
        private:
          RK4Integrator e_;

          VectorXd state_;

          Vector3d start_;
          Vector3d goal_;

          std::mt19937 gen_;
          std::uniform_real_distribution<double> xy_dis_;
          std::uniform_real_distribution<double> th_dis_;
      };


    } // namespace physics
  } // namespace physics
} // namespace cannon

#endif /* ifndef CANNON_PHYSICS_SYSTEMS_KINEMATIC_CAR_H */
