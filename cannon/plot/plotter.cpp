#include <cannon/plot/plotter.hpp>

using namespace cannon::plot;
using namespace cannon::log;

void Plotter::render() {
  w_.render_loop([this] {draw_pass();});
}

std::shared_ptr<Scatter> Plotter::plot_points(MatrixX2f points, Vector4f color, float size) {
  auto s = std::make_shared<Scatter>(point_program_, points, color, size);
  scatter_plots_.push_back(s);

  // Update plot extent
  RowVector2f maxes = points.colwise().maxCoeff();
  RowVector2f mins = points.colwise().minCoeff();
  axes_.update_limits(mins[0], maxes[0], mins[1], maxes[1]);

  return s;
}

std::shared_ptr<Line> Plotter::plot_line(MatrixX2f points, Vector4f color) {
  auto l = std::make_shared<Line>(line_program_, points, color);
  line_plots_.push_back(l);

  // Update plot extent
  RowVector2f maxes = points.colwise().maxCoeff();
  RowVector2f mins = points.colwise().minCoeff();
  axes_.update_limits(mins[0], maxes[0], mins[1], maxes[1]);

  return l;
}

void Plotter::save(const std::string &path) {
  w_.render_once([this] {draw_pass();});
  w_.save_image(path);
}

void Plotter::draw_pass() {
  axes_.draw();

  point_program_->set_uniform("matrix", axes_.make_scaling_matrix());
  for (auto& scatter : scatter_plots_) {
    scatter->draw();
  }

  line_program_->set_uniform("matrix", axes_.make_scaling_matrix());
  for (auto& line : line_plots_) {
    line->draw();
  }

  // TODO
}
