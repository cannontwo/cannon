#include <cannon/plot/plotter.hpp>

using namespace cannon::plot;
using namespace cannon::log;

void Plotter::render() {
  program_.activate();
  w_.render_loop([this] {
      // Set line color to black by default
      program_.set_uniform("uColor", Vector4f(0.0, 0.0, 0.0, 1.0));
      
      axes_.draw();

      for (auto& scatter : scatter_plots_) {
        scatter.draw(axes_.make_scaling_matrix());
      }

      // TODO
      });
}

void Plotter::plot_points(MatrixX2f points, Vector4f color, float size) {
  scatter_plots_.emplace_back(points, color, size);

  // Update plot extent
  RowVector2f maxes = points.colwise().maxCoeff();
  RowVector2f mins = points.colwise().minCoeff();
  axes_.update_limits(mins[0], maxes[0], mins[1], maxes[1]);
}
