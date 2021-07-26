#ifdef CANNON_BUILD_GRAPHICS

#include <cmath>

#include <cannon/log/registry.hpp>
#include <cannon/plot/plotter.hpp>
#include <cannon/math/random_double.hpp>

using namespace cannon::plot;
using namespace cannon::log;
using namespace cannon::math;

int main() {
  Plotter plotter;

  for (unsigned int i = 0; i < 20; ++i) {
    std::vector<Vector2d> pts;

    pts.emplace_back(random_double(), random_double());
    pts.emplace_back(random_double(), random_double());

    plotter.plot(pts);
  }

  plotter.render();
}
#else
int main() {}
#endif
