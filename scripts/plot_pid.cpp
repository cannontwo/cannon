#ifdef CANNON_BUILD_GRAPHICS

#include <imgui.h>

#include <cannon/control/pid.hpp>
#include <cannon/log/registry.hpp>
#include <cannon/physics/rk4_integrator.hpp>
#include <cannon/plot/plotter.hpp>

using namespace cannon::plot;

using namespace cannon::log;
using namespace cannon::control;

static const double m = 1;
static const double b = 10;
static const double k = 20;

void step(VectorXd& s, double u, double timestep) {
  VectorXd old_s = s;
  s[2] = (u - k*old_s[0] - b*old_s[1]) / m;
  s[1] += timestep * old_s[2];
  s[0] += timestep * old_s[1];
}

std::vector<Vector2d> plot_pid_traj(double ref, double kp, double ki, double kd, double timestep) {
  PidController pid(1, 1, timestep);

  pid.ref()[0] = ref;
  pid.proportional_gain()(0, 0) = kp;
  pid.integral_gain()(0, 0) = ki;
  pid.derivative_gain()(0, 0) = kd;

  double time = 0.0;
  VectorXd state = VectorXd::Zero(3);

  std::vector<Vector2d> pts;
  for (unsigned int i = 0; i < 2.0 / timestep; ++i) {
    Vector2d pt;
    pt << time,
          state[0];
    pts.push_back(pt);

    VectorXd pid_state(1);
    pid_state[0] = state[0];
    VectorXd pid_control = pid.get_control(pid_state);

    step(state, pid_control[0], timestep);

    time += timestep;
  }

  return pts;
}

int main() {
  Plotter plotter;

  plotter.render([&]() {
    static float ref = 1.0;
    static float kp = 0.0;
    static float ki = 0.0;
    static float kd = 0.0;
    static float timestep = 0.01;

    bool changed = false;
    if (ImGui::BeginMainMenuBar()) {
      if (ImGui::BeginMenu("PID")) {
        changed = changed || ImGui::InputFloat("Reference", &ref);
        changed = changed || ImGui::InputFloat("Integration Timestep", &timestep);
        changed = changed || ImGui::SliderFloat("Proportional Gain", &kp, 0.0, 1000.0);
        changed = changed || ImGui::SliderFloat("Integral Gain", &ki, 0.0, 1000.0);
        changed = changed || ImGui::SliderFloat("Derivative Gain", &kd, 0.0, 1000.0);
        
        ImGui::EndMenu();
      }
      ImGui::EndMainMenuBar();
    }

    if (changed) {
      plotter.clear();
      auto pts = plot_pid_traj(ref, kp, ki, kd, timestep);
      plotter.plot(pts);
    }
  });
}
#else
int main() {}
#endif
