#ifndef CANNON_PHYSICS_SYSTEMS_DYNAMIC_CAR_H
#define CANNON_PHYSICS_SYSTEMS_DYNAMIC_CAR_H 

#include <random>
#include <cassert>

#include <ompl/control/ODESolver.h>
#include <ompl/control/spaces/RealVectorControlSpace.h>
#include <ompl/base/spaces/SO2StateSpace.h>
#include <ompl/base/spaces/SE2StateSpace.h>

#include <Eigen/Dense>

#include <cannon/physics/rk4_integrator.hpp>
#include <cannon/physics/systems/system.hpp>
#include <cannon/graphics/geometry/plane.hpp>
#include <cannon/log/registry.hpp>

using namespace Eigen;

using namespace cannon::log;

namespace oc = ompl::control;
namespace ob = ompl::base;

namespace cannon {
  namespace physics {
    namespace systems {

      struct DynamicCarSystem : System {
        DynamicCarSystem(double l = 1.0) : l_(l) {}

        virtual void operator()(const VectorXd& s, VectorXd& dsdt, const double /*t*/) override {
          double th = s[2];
          double v = s[3];
          double dth = s[4];
          double ua = s[5];
          double uth = s[6];

          dsdt.resize(7);
          dsdt[0] = v * std::cos(th);
          dsdt[1] = v * std::sin(th);
          dsdt[2] = (v / l_) * std::tan(dth);
          dsdt[3] = ua;
          dsdt[4] = uth;
          dsdt[5] = 0.0;
          dsdt[6] = 0.0;
        }

        virtual void ompl_ode_adaptor(const oc::ODESolver::StateType& q, 
            const oc::Control* control, oc::ODESolver::StateType& qdot) override {

          const double ua = control->as<oc::RealVectorControlSpace::ControlType>()->values[0];
          const double uth = control->as<oc::RealVectorControlSpace::ControlType>()->values[1];

          VectorXd s(7);
          s[0] = q[0];
          s[1] = q[1];
          s[2] = q[2];
          s[3] = q[3];
          s[4] = q[4];
          s[5] = ua;
          s[6] = uth;
          VectorXd dsdt(7);

          (*this)(s, dsdt, 0.0);

          qdot.resize(q.size(), 0);
          for (unsigned int i = 0; i < q.size(); i++) {
            qdot[i] = dsdt[i];
          }
        }

        virtual std::tuple<MatrixXd, MatrixXd, VectorXd> get_linearization(const VectorXd& x) override {
          assert(x.size() == 5);

          double th = x[2];
          double v = x[3];
          double dth = x[4];

          // TODO Don't hardcode timestep at some point
          double timestep = 0.01;

          MatrixXd A(5, 5);
          A << 1, 0, -v * std::sin(th) * timestep, std::cos(th) * timestep, 0,
               0, 1, v * std::cos(th) * timestep, std::sin(th) * timestep, 0,
               0, 0, 1, (tan(dth) / l_) * timestep, (v / l_) * (1.0 / (cos(dth) * cos(dth))) * timestep,
               0, 0, 0, 1, 0,
               0, 0, 0, 0, 1;

          MatrixXd B(5, 2);
          B << 0, 0,
               0, 0,
               0, 0,
               timestep, 0,
               0, timestep;

          
          VectorXd c(5); 
          c << x[0] + v * std::cos(th) * timestep,
               x[1] + v * std::cos(th) * timestep,
               th + (v / l_) * tan(dth) * timestep,
               v,
               dth;

          // We linearize around u = 0, so no additional term is subtracted from c
          return std::make_tuple(A, B, c - A * x);
        }

        virtual void
        get_continuous_time_linearization(const oc::ODESolver::StateType &q,
                                          Ref<MatrixXd> A, Ref<MatrixXd> B) override {
          double c = cos(q[2]), s = sin(q[2]);
          A = 1e-3 * Eigen::MatrixXd::Identity(5, 5);
          A(0, 2) = -q[3] * s;
          A(0, 3) = c;
          A(1, 2) = q[3] * c;
          A(1, 3) = s;
          A(2, 3) = (1.0 / l_) * tan(q[4]);
          A(2, 4) = q[3] * (1.0 / l_) / (cos(q[4]) * cos(q[4]));

          B = Eigen::MatrixXd::Zero(5, 2);
          B(3, 0) = B(4, 1) = 1.;
        }

        static void ompl_post_integration(const ob::State* /*state*/, const
            oc::Control* /*control*/, const double /*duration*/, ob::State *result) {

          ob::SO2StateSpace SO2;
          SO2.enforceBounds(result->as<ob::CompoundStateSpace::StateType>()
                                ->as<ob::SE2StateSpace::StateType>(0)
                                ->as<ob::SO2StateSpace::StateType>(1));
        }

        // Parameters
        double l_;
      };

      class DynamicCar {
        public:
          DynamicCar() = delete;

          DynamicCar(Vector3d s, Vector3d g) : e_(s_, 7, time_step), start_(s), goal_(g) {
            std::random_device rd;
            gen_ = std::mt19937(rd());  

            xy_dis_ = std::uniform_real_distribution<double>(-1.0, 1.0);
            th_dis_ = std::uniform_real_distribution<double>(-M_PI, M_PI);

            state_ = VectorXd::Zero(7);
            reset();
          }

          std::pair<VectorXd, double> step(double ua, double uth) {
            //double clipped_uth = std::max(-M_PI * 2.0 / 180.0, std::min(uth, M_PI * 2.0 / 180.0));
            double clipped_uth = std::max(-M_PI, std::min(uth, M_PI));
           
            state_[5] = ua;
            state_[6] = clipped_uth;

            double goal_r = -std::pow((state_.head(2) - goal_.head(2)).norm(), 2.0);
            double control_r = -std::pow((std::abs(clipped_uth) + std::abs(ua)), 2.0);
            double reward = goal_r + 0.001*control_r;

            e_.set_state(state_);
            state_ = e_.step();

            return std::make_pair(state_.head(5), reward);
          }
          
          VectorXd reset() {
            state_[0] = xy_dis_(gen_);
            state_[1] = xy_dis_(gen_);
            state_[2] = th_dis_(gen_);
            state_[3] = 0.0;
            state_[4] = 0.0;
            
            state_[4] = 0.0;
            state_[5] = 0.0;

            return state_.head(5);
          }

          
          VectorXd reset(const VectorXd& s) {
            assert(s.size() == 5);

            state_[0] = s[0];
            state_[1] = s[1];
            state_[2] = s[2];
            state_[3] = s[3];
            state_[4] = s[4];
            
            state_[5] = 0.0;
            state_[6] = 0.0;

            return state_.head(5);
          }

          // In seconds
          const double time_step = 0.01;

          DynamicCarSystem s_;
          
        private:
          RK4Integrator e_;

          VectorXd state_;

          VectorXd start_;
          VectorXd goal_;

          std::mt19937 gen_;
          std::uniform_real_distribution<double> xy_dis_;
          std::uniform_real_distribution<double> th_dis_;
      };


    } // namespace physics
  } // namespace physics
} // namespace cannon

#endif /* ifndef CANNON_PHYSICS_SYSTEMS_DYNAMIC_CAR_H */
