#ifndef CANNON_PHYSICS_SYSTEMS_SYSTEM_H
#define CANNON_PHYSICS_SYSTEMS_SYSTEM_H 

#include <Eigen/Dense>

#include <ompl/config.h>
#include <ompl/control/ODESolver.h>

namespace oc = ompl::control;
namespace ob = ompl::base;

using namespace Eigen;

namespace cannon {
  namespace physics {
    namespace systems {

      class System {
        public:
          virtual void operator()(const VectorXd& s, VectorXd& dsdt, const double t) = 0;

          virtual void ompl_ode_adaptor(const oc::ODESolver::StateType& q, 
              const oc::Control* control, oc::ODESolver::StateType& qdot) = 0;

          virtual std::tuple<MatrixXd, MatrixXd, VectorXd> get_linearization(const VectorXd& x) = 0;
      };

    } // namespace system
  } // namespace physics
} // namespace cannon

#endif /* ifndef CANNON_PHYSICS_SYSTEMS_SYSTEM_H */
