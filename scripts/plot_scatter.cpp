#include <cannon/log/registry.hpp>

#include <cannon/plot/plotter.hpp>

using namespace cannon::plot;
using namespace cannon::log;

int main() {
  Plotter plotter;

  // TODO
  MatrixX2f pts = MatrixX2f::Random(10, 2);
  log_info(pts);
  plotter.plot_points(pts, Vector4f(0.0, 0.0, 0.0, 1.0), 15.0);

  MatrixX2f pts2(4, 2);
  pts2 << -5.0, 5.0,
          4.0, -6.0,
          10.0, 2.0,
          -10.0, -10.0;
  plotter.plot_points(pts2, Vector4f(1.0, 0.0, 0.0, 1.0), 15.0);

  plotter.render();
}
