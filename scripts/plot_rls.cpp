#ifdef CANNON_BUILD_GRAPHICS

#include <cmath>

#include <Eigen/Dense>

#include <cannon/log/registry.hpp>

#include <cannon/plot/plotter.hpp>
#include <cannon/plot/line.hpp>
#include <cannon/plot/scatter.hpp>

#include <cannon/ml/rls.hpp>

using namespace cannon::plot;
using namespace cannon::log;
using namespace cannon::ml;

int main() {
  Plotter plotter(true);

  // Initial line
  MatrixX2f pts(2, 2);
  pts <<  -1.0, 0.0,
          1.0, 0.0;

  std::shared_ptr<Line> l = plotter.plot_line(pts, Vector4f(0.0, 0.0, 0.0, 1.0));
  std::shared_ptr<Scatter> s = plotter.plot_points(pts, Vector4f(1.0, 0.0, 0.0, 1.0), 15.0);

  plotter.set_xlim(-2.0, 2.0);
  plotter.set_ylim(-2.0, 2.0);

  float t_delta = 1.0 / 24.0;
  float last_time = 0.0;

  RLSFilter f(1, 1);

  plotter.render([&f, &l, &s, &t_delta, &last_time] {
      float t = (float)glfwGetTime();
      if (t < last_time + t_delta) {
        return;
      }

      VectorXd x = VectorXd::Random(1);
      VectorXd y = 2.0 * x + VectorXd::Random(1);
      MatrixX2f new_pt(1, 2);
      new_pt << x[0], y[0];

      f.process_datum(x, y);
      s->add_points(new_pt);

      VectorXd tmp_x1(1);
      tmp_x1 << -2.0;
      VectorXd tmp_x2(1);
      tmp_x2 << 2.0;

      float x1 = -2.0;
      float y1 = f.predict(tmp_x1)[0];
      float x2 = 2.0;
      float y2 = f.predict(tmp_x2)[0];

      MatrixX2f new_pts(2, 2);
      new_pts << x1, y1,
                 x2, y2;
      l->replace_points(new_pts);

      last_time = t;
      });
}
#else
int main() {}
#endif
