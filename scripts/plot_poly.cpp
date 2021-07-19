#ifdef CANNON_BUILD_GRAPHICS

#include <imgui.h>

#include <cannon/log/registry.hpp>
#include <cannon/plot/plotter.hpp>
#include <cannon/math/polynomial.hpp>

using namespace cannon::plot;
using namespace cannon::log;
using namespace cannon::math;

int main(int argc, char** argv) {
  Plotter plotter(true);

  std::vector<double> coeffs;
  for (int i = 1; i < argc; ++i) {
    coeffs.push_back(std::stod(argv[i]));
  }

  Polynomial<MonomialBasis> p(coeffs);

  plotter.render([&] {
    plotter.clear();

    static int samples = 100;
    if (ImGui::BeginMainMenuBar()) {
      if (ImGui::BeginMenu("Plotting")) {
        ImGui::InputInt("Samples", &samples);
      }
    }

    plotter.plot(p, samples);
  });
}
#else
int main() {}
#endif
