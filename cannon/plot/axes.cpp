#include <cannon/plot/axes.hpp>

using namespace cannon::plot;

void Axes::draw() {
  program_.activate();
  program_.set_uniform("uColor", Vector4f(0.0, 0.0, 0.0, 1.0));
  buf_.bind();
  
  glDrawArrays(GL_LINES, 0, 4);
}

void Axes::update_limits(float x_min, float x_max, float y_min, float y_max) {
  x_min_ = std::min(x_min, x_min_);
  y_min_ = std::min(y_min, y_min_);
  x_max_ = std::max(x_max, x_max_);
  y_max_ = std::max(y_max, y_max_);

  MatrixX2f lines(4, 2);
  lines << x_min_, 0.0,
           x_max_, 0.0,
           0.0, y_min_, 
           0.0, y_max_; 
  buf_.buffer(lines);
}
