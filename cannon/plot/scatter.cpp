#include <cannon/plot/scatter.hpp>

using namespace cannon::plot;
using namespace cannon::log;

void Scatter::add_points(MatrixX2f point) {
  points_.conservativeResize(points_.rows() + 1, NoChange);
  points_.row(points_.rows() - 1) = point;

  buf_.buffer(points_);
  RowVector2f maxes = points_.colwise().maxCoeff();
  RowVector2f mins = points_.colwise().minCoeff();
  plotter_.set_xlim(mins[0], maxes[0]);
  plotter_.set_ylim(mins[1], maxes[1]);
}

void Scatter::draw() {
  program_->set_uniform("uColor", color_);
  program_->set_uniform("pointSize", point_size_);
  program_->activate();

  buf_.bind();
  glDrawArrays(GL_POINTS, 0, points_.rows());
}

