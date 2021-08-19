#ifdef CANNON_BUILD_GRAPHICS

#include <cannon/math/interp.hpp>
#include <cannon/plot/plotter.hpp>

using namespace cannon::math;
using namespace cannon::plot;

int main() {
  std::vector<double> xs, ys;

  xs.push_back(0);
  ys.push_back(21);
  xs.push_back(1);
  ys.push_back(24);
  xs.push_back(2);
  ys.push_back(24);
  xs.push_back(3);
  ys.push_back(18);
  xs.push_back(4);
  ys.push_back(16);

  
  MatrixX2f plot_points = MatrixX2f::Zero(xs.size(), 2);
  for (unsigned int i = 0; i < xs.size(); ++i) {
    plot_points(i, 0) = xs[i];
    plot_points(i, 1) = ys[i];
  }

  CubicSpline spline_second_deriv(xs, ys);
  CubicSpline spline_first_deriv(xs, ys, CubicSpline::BoundaryType::FirstDeriv);
  CubicSpline spline_not_knot(xs, ys, CubicSpline::BoundaryType::NotAKnot);

  Plotter plotter;
  plotter.plot(spline_second_deriv, 200, -1.0, 5.5);
  plotter.plot(spline_first_deriv, 200, -1.0, 5.5);
  plotter.plot(spline_not_knot, 200, -1.0, 5.5);
  plotter.plot_points(plot_points);
 
  plotter.render();
}
#else
int main() {}
#endif
