#include <cannon/plot/axes.hpp>

using namespace cannon::plot;

void Axes::draw() {
  program_.activate();
  program_.set_uniform("uColor", Vector4f(0.0, 0.0, 0.0, 1.0));
  program_.set_uniform("matrix", make_scaling_matrix());
  buf_.bind();
  
  glDrawArrays(GL_LINES, 0, 4);
}

void Axes::update_limits(float x_min, float x_max, float y_min, float y_max) {
  x_min_ = std::min(x_min, x_min_);
  y_min_ = std::min(y_min, y_min_);
  x_max_ = std::max(x_max, x_max_);
  y_max_ = std::max(y_max, y_max_);

  Vector2f scaled_padding = get_scaled_padding();

  MatrixX2f lines(4, 2);
  lines << x_min_ - scaled_padding[0], 0.0,
           x_max_ + scaled_padding[0], 0.0,
           0.0, y_min_ - scaled_padding[1], 
           0.0, y_max_ + scaled_padding[1]; 
  buf_.buffer(lines);
}

Matrix4f Axes::make_scaling_matrix() {
  Matrix2f extent = get_padded_extent(); 

  Vector3f center = {(extent(0,0) + extent(0, 1))/2.0f, (extent(1, 0) + extent(1, 1))/2.0f, 0.0f};
  float scale_x = 2.0f / (extent(0, 1) - extent(0, 0));
  float scale_y = 2.0f / (extent(1, 1) - extent(1, 0));
  Vector3f scale = {scale_x, scale_y, 1.0};

  Affine3f t(Affine3f::Identity());
  t.scale(scale);
  t.translate(-center);
  return t.matrix();
}

Vector2f Axes::get_scaled_padding() {
  Vector2f p;
  p << ((x_max_ - x_min_) / 2.0) * padding_,
       ((y_max_ - y_min_) / 2.0) * padding_;

  return p;
}

Matrix2f Axes::get_padded_extent() {
  Vector2f p = get_scaled_padding();
  Matrix2f e;
  e << x_min_ - p[0], x_max_ + p[0],
       y_min_ - p[1], y_max_ + p[1];

  return e;
}
