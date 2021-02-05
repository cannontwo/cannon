#include <cannon/plot/plotter.hpp>

using namespace cannon::plot;
using namespace cannon::log;

void Plotter::close() const {
  w_.close();
}

ImageData Plotter::get_image() const {
  return w_.get_image();
}

void Plotter::render() {
  w_.render_loop([this] {
        draw_pass();
      });
}

std::shared_ptr<Scatter> Plotter::plot_points(MatrixX2f points, Vector4f color, float size) {
  auto s = std::make_shared<Scatter>(*this, point_program_, points, color, size);
  scatter_plots_.push_back(s);

  // Update plot extent
  RowVector2f maxes = points.colwise().maxCoeff();
  RowVector2f mins = points.colwise().minCoeff();
  axes_.update_limits(mins[0], maxes[0], mins[1], maxes[1], w_.width, w_.height);

  return s;
}

std::shared_ptr<Line> Plotter::plot_line(MatrixX2f points, Vector4f color) {
  auto l = std::make_shared<Line>(*this, line_program_, points, color);
  line_plots_.push_back(l);

  // Update plot extent
  RowVector2f maxes = points.colwise().maxCoeff();
  RowVector2f mins = points.colwise().minCoeff();
  axes_.update_limits(mins[0], maxes[0], mins[1], maxes[1], w_.width, w_.height);

  return l;
}

std::shared_ptr<Polygon> Plotter::plot_polygon(const Polygon_2& poly, Vector4f color) {
  auto p = std::make_shared<Polygon>(*this, poly_program_, poly, color);
  polygon_plots_.push_back(p);

  RowVector2f maxes = p->points_.colwise().maxCoeff();
  RowVector2f mins = p->points_.colwise().minCoeff();
  axes_.update_limits(mins[0], maxes[0], mins[1], maxes[1], w_.width, w_.height);

  return p;
}

void Plotter::set_xlim(float low, float high) {
  axes_.update_limits(low, high, 0.0, 0.0, w_.width, w_.height);
}

void Plotter::set_ylim(float low, float high) {
  axes_.update_limits(0.0, 0.0, low, high, w_.width, w_.height);
}

void Plotter::draw_pass() {
  axes_.update_scale(w_.width, w_.height);
  axes_.draw();

  point_program_->set_uniform("matrix", axes_.make_scaling_matrix());
  for (auto& scatter : scatter_plots_) {
    scatter->draw();
  }

  line_program_->set_uniform("matrix", axes_.make_scaling_matrix());
  for (auto& line : line_plots_) {
    line->draw();
  }

  poly_program_->set_uniform("matrix", axes_.make_scaling_matrix());
  for (auto& poly : polygon_plots_) {
    poly->draw();
  }

  // TODO
}

void Plotter::display_fps() {
  w_.display_fps(-25.0, -25.0, 1.0);
}
