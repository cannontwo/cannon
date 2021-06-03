#ifdef CANNON_BUILD_GRAPHICS

#include <cannon/log/registry.hpp>

#include <cannon/plot/plotter.hpp>
#include <cannon/plot/scatter.hpp>
#include <cannon/math/multivariate_normal.hpp>

using namespace cannon::plot;
using namespace cannon::log;
using namespace cannon::math;

int main() {
  Plotter plotter;
  plotter.display_fps();

  Matrix2d covar = Matrix2d::Identity();
  covar(0, 0) = 2.0;

  MultivariateNormal dist(covar);

  MatrixX2f pts = dist.sample().transpose().cast<float>() * 0.1;
  log_info(pts);
  auto scatter = plotter.plot_points(pts, Vector4f(0.0, 0.0, 0.0, 1.0), 15.0);

  double last_time = glfwGetTime();
  double time_delta = 0.01;

  plotter.render([&] {
      if (glfwGetTime() > last_time + time_delta) {
        MatrixX2f new_pt = dist.sample().transpose().cast<float>() * 0.1;
        scatter->add_points(new_pt);
        last_time = glfwGetTime();
      }
      });
}
#else
int main() {}
#endif
