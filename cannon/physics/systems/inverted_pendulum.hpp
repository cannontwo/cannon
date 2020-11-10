#ifndef CANNON_PHYSICS_SYSTEMS_INVERTED_PENDULUM_H
#define CANNON_PHYSICS_SYSTEMS_INVERTED_PENDULUM_H 

#include <cmath>
#include <utility>
#include <random>

#include <Eigen/Dense>

#include <cannon/physics/euler_integrator.hpp>
#include <cannon/log/registry.hpp>

using namespace Eigen;

using namespace cannon::log;

namespace cannon {
  namespace physics {
    namespace systems {

      struct PendSystem {
        PendSystem(double g = 10.0, double m = 1.0, double l = 1.0, double dt =
            0.05, double max_speed = 8.0) : g_(g), m_(m), l_(l), dt_(dt),
        max_speed_(max_speed) {}

        void operator()(const VectorXd& x, VectorXd& dxdt, const double /*t*/) {
          double th = x[0];
          double thdot = x[1];
          double u = x[2];

          double new_thdot = (-3 * g_ / (2 * l_)) * std::sin(th + M_PI) + 
            (3.0 * u / (m_ * std::pow(l_, 2.0)));
          double new_th = thdot + (new_thdot * dt_);

          dxdt.resize(3);
          dxdt[0] = new_th;
          dxdt[1] = new_thdot;
          dxdt[2] = 0.0;
        }

        // Parameters
        double g_;
        double m_;
        double l_;
        double dt_;
        double max_speed_;
      };

      class InvertedPendulum {
        public:
          InvertedPendulum(double max_torque = 2.0) : max_torque_(max_torque),
          e_(s_, 3, 0.05) {
            std::random_device rd;
            gen_ = std::mt19937(rd());  

            th_dis_ = std::uniform_real_distribution<double>(-M_PI, M_PI);
            thdot_dis_ = std::uniform_real_distribution<double>(-1.0, 1.0);

            state_ = Vector3d::Zero(3);
            reset(); 
          }

          std::pair<VectorXd, double> step(double u) {
            double th = state_[0];
            double thdot = state_[1];

            double reward = -(std::pow(normalize_(th), 2.0) + (0.1 * std::pow(thdot,
                    2.0)) + (0.001 * std::pow(u, 2.0)));

            double clipped_u = std::max(-max_torque_, std::min(max_torque_, u));
            state_[2] = clipped_u;

            e_.set_state(state_);
            state_ = e_.step();

            state_[0] = std::atan2(std::sin(state_[0]),std::cos(state_[0]));
            state_[1] = std::max(-8.0, std::min(state_[1], 8.0)); 
            
            return std::make_pair(state_.head(2), reward);
          }

          VectorXd reset() {
            double th = th_dis_(gen_);
            double thdot = thdot_dis_(gen_);

            state_[0] = th;
            state_[1] = thdot;
            state_[2] = 0.0;

            return state_.head(2);
          }

        private:
          inline double normalize_(double th) {
            return (std::fmod((th + M_PI), (2.0 * M_PI)) - M_PI); 
          }

          double max_torque_;

          PendSystem s_;
          EulerIntegrator<PendSystem> e_;

          Vector3d state_;

          std::mt19937 gen_;
          std::uniform_real_distribution<double> th_dis_;
          std::uniform_real_distribution<double> thdot_dis_;
      };

    } // namespace systems
  } // namespace physics
} // namespace cannon

#endif /* ifndef CANNON_PHYSICS_SYSTEMS_INVERTED_PENDULUM_H */
