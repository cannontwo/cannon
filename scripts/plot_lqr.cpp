#ifdef CANNON_BUILD_GRAPHICS

#include <cmath>

#include <imgui.h>

#include <cannon/log/registry.hpp>

#include <cannon/plot/plotter.hpp>
#include <cannon/plot/line.hpp>
#include <cannon/control/lqr.hpp>
#include <cannon/physics/rk4_integrator.hpp>

using namespace cannon::plot;
using namespace cannon::log;
using namespace cannon::control;
using namespace cannon::physics;

void double_int_system(const VectorXd& q, VectorXd& qdot, double t) {
  static Matrix4d A = (Matrix4d() << 0, 0, 1, 0,
                                     0, 0, 0, 1,
                                     0, 0, 0, 0,
                                     0, 0, 0, 0).finished();
  static MatrixXd B = (MatrixXd(4, 2) << 0, 0, 
                                         0, 0, 
                                         1, 0, 
                                         0, 1).finished();

  qdot.resize(6);
  qdot.head(4) = A * q.head(4) + B * q.tail(2);
  
  qdot.tail(2) = Vector2d::Zero();
}

void double_int_lin(const Ref<const VectorXd> &q, Ref<MatrixXd> A,
                    Ref<MatrixXd> B) {
  A.resize(4, 4);
  A << 0, 0, 1, 0,
       0, 0, 0, 1,
       0, 0, 0, 0,
       0, 0, 0, 0;

  B.resize(4, 2);
  B << 0, 0,
       0, 0,
       1, 0,
       0, 1;
}

void controlled_double_int_system(const VectorXd& q, VectorXd& qdot, double t) {
  static VectorXd goal = (VectorXd(4) << -1.0, 0.0, 0.0, 0.0).finished();
  static LQRController controller(goal, 2, double_int_lin);

  auto control = controller.compute_control(q);
  VectorXd aug_state(6);
  aug_state.head(4) = q;
  aug_state.tail(2) = control;

  double_int_system(aug_state, qdot, t);
}

int main() {
  Plotter plotter;

  MatrixX2f pts(1, 2);
  pts << 1.0, 1.0;

  log_info(pts);
  std::shared_ptr<Line> l = plotter.plot_line(pts, Vector4f(0.0, 0.0, 0.0, 1.0));

  plotter.set_xlim(-5.0, 5.0);
  plotter.set_ylim(-5.0, 5.0);

  Vector4d state;
  state << 1.0,
           1.0,
           -1.0,
           0.0;

  double last_time = glfwGetTime();
  double timestep = 0.01;

  RK4Integrator integrator(controlled_double_int_system, 4, timestep);
  integrator.set_state(state);

  Vector4d goal = Vector4d::Zero();
  goal[0] = -1.0;

  MatrixX2f goal_pt(1, 2);
  goal_pt << goal[0],
             goal[1];
  plotter.plot_points(goal_pt, Vector4f(1.0, 0.0, 0.0, 1.0), 15.0);

  plotter.render([&] {
    if (glfwGetTime() > last_time + timestep) {
      MatrixX2f new_pts(1, 2);
      new_pts << state[0], state[1];
      l->add_points(new_pts);

      state = integrator.step();
      last_time = glfwGetTime();
    }
  });
}
#else
int main() {}
#endif
