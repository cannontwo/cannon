#include <cannon/plot/line.hpp>

using namespace cannon::plot;
using namespace cannon::log;

void Line::add_points(MatrixX2f point) {
  points_.conservativeResize(points_.rows() + 1, NoChange);
  points_.row(points_.rows() - 1) = point;

  buf_.buffer(points_);
  update_plotter();
}

void Line::replace_points(MatrixX2f points) {
  points_ = points;
  buf_.replace(points_);
  update_plotter();
}

void Line::draw() {
  program_->activate();
  program_->set_uniform("uColor", color_);

  buf_.bind();
  glLineWidth(2.0);
  glDrawArrays(GL_LINE_STRIP, 0, points_.rows());
  glLineWidth(1.0);
}

void Line::update_plotter() {
  RowVector2f maxes = points_.colwise().maxCoeff();
  RowVector2f mins = points_.colwise().minCoeff();
  plotter_.set_xlim(mins[0], maxes[0]);
  plotter_.set_ylim(mins[1], maxes[1]);
}
