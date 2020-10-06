#include <cannon/plot/plotter.hpp>

using namespace cannon::plot;

void Plotter::render() {
  program_.activate();
  w_.render_loop([this] {
      // Set line color to black by default
      program_.set_uniform("uColor", Vector4f(0.0, 0.0, 0.0, 1.0));
      // TODO
      });
}
