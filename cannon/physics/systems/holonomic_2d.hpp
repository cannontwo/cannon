#ifndef CANNON_PHYSICS_SYSTEMS_INVERTED_PENDULUM_H
#define CANNON_PHYSICS_SYSTEMS_INVERTED_PENDULUM_H 

#include <random>

#include <Eigen/Dense>

#include <cannon/physics/rk4_integrator.hpp>

using namespace Eigen;

namespace cannon {
  namespace physics {
    namespace systems {

      struct Hol2DSystem {
        Hol2DSystem() {}

        void operator()(const VectorXd& s, VectorXd& dsdt, const double /*t*/) {
          double ux = s[2];
          double uy = s[3];

          dsdt.resize(4);
          dsdt[0] = ux;
          dsdt[1] = uy;
          dsdt[2] = 0.0;
          dsdt[3] = 0.0;
        }

        // Parameters?
        
      };

      class Holonomic2D {
        public:
          Holonomic2D() = delete;

          Holonomic2D(Vector2d s, Vector2d g) : e_(s_, 4, 0.01), start_(s), goal_(g) {
            std::random_device rd;
            gen_ = std::mt19937(rd());

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

        private:
          Hol2DSystem s_;
          RK4Integrator e_;

          Vector4d state_;

          Vector2d start_;
          Vector2d goal_;

          std::mt19937 gen_;
          std::uniform_real_distribution<double> dis0_;
          std::uniform_real_distribution<double> dis1_;

      };

    } // namespace systems
  } // namespace physics
} // namespace cannon

#endif /* ifndef CANNON_PHYSICS_SYSTEMS_INVERTED_PENDULUM_H */
