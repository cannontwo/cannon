#ifdef CANNON_BUILD_GRAPHICS

#include <imgui.h>

#include <cannon/log/registry.hpp>
#include <cannon/plot/plotter.hpp>

using namespace cannon::plot;
using namespace cannon::log;

double f(const Vector2d& x) {
  Matrix2d A;
  A << 10.0, 0.0,
       0.0, 1.0;
  auto tmp = x.transpose() * A * x;
  return tmp(0, 0);
}

int main(int argc, char** argv) {
  Plotter plotter(true);

  plotter.plot(f);

  plotter.render();
}
#else
int main() {}
#endif
