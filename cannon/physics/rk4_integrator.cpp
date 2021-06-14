#include <cannon/physics/rk4_integrator.hpp>

using namespace cannon::physics;


VectorXd RK4Integrator::step() {
  //stepper_.do_step(system_, state_, t_, dt_);
  auto solver = boost::numeric::odeint::make_controlled(1e-6, 1e-6, stepper_type());
  boost::numeric::odeint::integrate_adaptive(solver, system_, state_, 0.0, dt_, 1e-3);
  t_ += dt_;

  return state_;
}

void RK4Integrator::set_state(const VectorXd& state) {
  state_ = state;
}

VectorXd RK4Integrator::get_state() const {
  return state_;
}

void RK4Integrator::set_time(double t) {
  t_ = t;
}

double RK4Integrator::get_time() {
  return t_;
}
