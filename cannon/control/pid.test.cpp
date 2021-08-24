#include <catch2/catch.hpp>

#include <cannon/control/pid.hpp>
#include <cannon/log/registry.hpp>
#include <cannon/physics/rk4_integrator.hpp>

#ifdef CANNON_BUILD_GRAPHICS
#include <cannon/plot/plotter.hpp>

using namespace cannon::plot;
#endif

using namespace cannon::log;
using namespace cannon::control;

static const double TIMESTEP = 0.001;
static const double m = 1;
static const double b = 10;
static const double k = 20;

void step(VectorXd& s, double u) {
  VectorXd old_s = s;
  s[2] = (u - k*old_s[0] - b*old_s[1]) / m;
  s[1] += TIMESTEP * old_s[2];
  s[0] += TIMESTEP * old_s[1];
}

TEST_CASE("PID", "[control]") {
  PidController pid(1, 1, TIMESTEP);

  pid.ref()[0] = 1.0;
  pid.proportional_gain()(0, 0) = 350.0;
  pid.integral_gain()(0, 0) = 300.0;
  pid.derivative_gain()(0, 0) = 50.0;

  double time = 0.0;
  VectorXd state = VectorXd::Zero(3);

  std::vector<Vector2d> pts;
  for (unsigned int i = 0; i < 2.0 / TIMESTEP; ++i) {
    Vector2d pt;
    pt << time,
          state[0];
    pts.push_back(pt);

    VectorXd pid_state(1);
    pid_state[0] = state[0];
    VectorXd pid_control = pid.get_control(pid_state);

    step(state, pid_control[0]);

    time += TIMESTEP;
  }

#ifdef CANNON_BUILD_GRAPHICS
  Plotter plotter;

  plotter.plot(pts);
  plotter.render();
#endif
}
