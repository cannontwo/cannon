#ifdef CANNON_BUILD_GRAPHICS

#include <cannon/log/registry.hpp>

#include <cannon/plot/plotter.hpp>
#include <cannon/plot/scatter.hpp>

using namespace cannon::plot;
using namespace cannon::log;

int main() {
  Plotter plotter;
  plotter.display_fps();

  MatrixX2f pts = MatrixX2f::Random(1, 2);
  log_info(pts);
  auto scatter = plotter.plot_points(pts, Vector4f(0.0, 0.0, 0.0, 1.0), 15.0);

  double last_time = glfwGetTime();
  double time_delta = 0.1;

  plotter.render([&] {
      if (glfwGetTime() > last_time + time_delta) {
        MatrixX2f new_pt = MatrixX2f::Random(1, 2);
        scatter->add_points(new_pt);
        last_time = glfwGetTime();
      }
      });
}
#else
int main() {}
#endif
