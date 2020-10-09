#include <cannon/plot/line.hpp>

using namespace cannon::plot;
using namespace cannon::log;

void Line::add_points(MatrixX2f point) {
  points_.resize(points_.rows() + 1, NoChange);
  points_.row(points_.rows() - 1) = point;

  buf_.buffer(points_);
}

void Line::replace_points(MatrixX2f points) {
  points_ = points;
  buf_.replace(points_);
}

void Line::draw() {
  program_->activate();
  program_->set_uniform("uColor", color_);

  buf_.bind();
  glLineWidth(2.0);
  glDrawArrays(GL_LINE_STRIP, 0, points_.rows());
  glLineWidth(1.0);
}
