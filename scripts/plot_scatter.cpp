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

  plotter.render();
}
