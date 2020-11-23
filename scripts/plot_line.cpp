#include <cmath>

#include <cannon/log/registry.hpp>

#include <cannon/plot/plotter.hpp>
#include <cannon/plot/line.hpp>

using namespace cannon::plot;
using namespace cannon::log;

int main() {
  Plotter plotter;

  MatrixX2f pts(2, 2);
  float t = glfwGetTime();
  pts << -5.0 * std::cos(t), -5.0 * std::sin(t),
          5.0 * std::cos(t), 5.0 * std::sin(t);

  log_info(pts);
  std::shared_ptr<Line> l = plotter.plot_line(pts, Vector4f(0.0, 0.0, 0.0, 1.0));

  plotter.set_xlim(-5.0, 5.0);
  plotter.set_ylim(-5.0, 5.0);

  plotter.render([&l] {
      float t = (float)glfwGetTime();
      float x1 = -5.0 * std::cos(t);
      float y1 = -5.0 * std::sin(t);
      float x2 = 5.0 * std::cos(t);
      float y2 = 5.0 * std::sin(t);

      MatrixX2f new_pts(2, 2);
      new_pts << x1, y1,
                 x2, y2;
      l->replace_points(new_pts);
      });
}
