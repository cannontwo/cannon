#include <cannon/plot/scatter.hpp>

using namespace cannon::plot;
using namespace cannon::log;

void Scatter::add_points(MatrixX2f point) {
  points_.resize(points_.rows() + 1, NoChange);
  points_.row(points_.rows() - 1) = point;

  buf_.buffer(points_);
}

void Scatter::draw() {
  program_->activate();
  program_->set_uniform("uColor", color_);
  program_->set_uniform("pointSize", point_size_);

  buf_.bind();
  glDrawArrays(GL_POINTS, 0, points_.rows());
}
