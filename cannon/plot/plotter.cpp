#include <cannon/plot/plotter.hpp>

using namespace cannon::plot;
using namespace cannon::log;

void Plotter::render() {
  w_.render_loop([this] {draw_pass();});
}

void Plotter::plot_points(MatrixX2f points, Vector4f color, float size) {
  scatter_plots_.emplace_back(point_program_, points, color, size);

  // Update plot extent
  RowVector2f maxes = points.colwise().maxCoeff();
  RowVector2f mins = points.colwise().minCoeff();
  axes_.update_limits(mins[0], maxes[0], mins[1], maxes[1]);
}

void Plotter::save(const std::string &path) {
  w_.render_once([this] {draw_pass();});
  w_.save_image(path);
}

void Plotter::draw_pass() {
  axes_.draw();

  point_program_->set_uniform("matrix", axes_.make_scaling_matrix());
  for (auto& scatter : scatter_plots_) {
    scatter.draw();
  }

  // TODO
}
