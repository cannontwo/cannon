#ifdef CANNON_BUILD_GRAPHICS
#include <cmath>

#include <cannon/math/interp.hpp>
#include <cannon/plot/plotter.hpp>

using namespace cannon::math;
using namespace cannon::plot;

double runge(double x) {
  return 1.0 / (1.0 + 25 * (x * x));
}

double step(double x) {
  return x > 0.0 ? 1.0 : 0.0;
}

int main() {
  unsigned int num_pts = 10;
  auto pts = cheb_points(num_pts);
  
  MatrixX2f plot_points = MatrixX2f::Zero(num_pts, 2);
  for (unsigned int i = 0; i < num_pts; ++i) {
    plot_points(i, 0) = pts[i];
    plot_points(i, 1) = runge(pts[i]);
  }

  std::vector<double> lin_pts;
  for (unsigned int i = 0; i < num_pts; ++i)
    lin_pts.push_back(-1.0 + (2.0 / (num_pts - 1)) * i);

  auto lin_interp = lagrange_interp(runge, lin_pts);
  auto cheb_interp = lagrange_interp(runge, pts);

  Plotter plotter;
  plotter.plot(runge, 200);
  plotter.plot(lin_interp, 200);
  plotter.plot(cheb_interp, 200);
  plotter.plot_points(plot_points);
 
  //plotter.plot([&](double x){return std::abs(runge(x) - cheb_interp(x));}, 200); 
  
  plotter.render();
}
#else
int main() {}
#endif
