#include <cannon/plot/axes.hpp>

using namespace cannon::plot;

void Axes::draw() {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  program_.set_uniform("uColor", Vector4f(0.0, 0.0, 0.0, 1.0));
  program_.set_uniform("matrix", make_scaling_matrix());
  program_.activate();
  buf_.bind();
  
  glDrawArrays(GL_LINES, 0, 4);

  text_program_.set_uniform("textColor", Vector4f(0.0, 0.0, 0.0, 1.0));
  text_program_.set_uniform("projection", make_scaling_matrix());
  text_program_.set_uniform("text", 0);
  text_program_.activate();

  for (auto& tick : x_ticks_) {
    float x = tick;
    std::string tick_s = get_tick_string(tick);
    std::string::const_iterator c;

    for (c = tick_s.begin(); c != tick_s.end(); c++) {
      std::shared_ptr<Character> ch = font_.get_char(*c);

      float xpos = x + ch->bearing[0] * text_scale_x_;
      float ypos = 0.0 - (ch->size[1] - ch->bearing[1]) * text_scale_y_;
      float w = ch->size[0] * text_scale_x_;
      float h = ch->size[1] * text_scale_y_;

      MatrixX4f vertices(6, 4);
      vertices << xpos, ypos + h,     0.0f, 0.0f,
                  xpos, ypos,         0.0f, 1.0f,
                  xpos + w, ypos,     1.0f, 1.0f,
                  xpos, ypos + h,     0.0f, 0.0f,
                  xpos + w, ypos,     1.0f, 1.0f,
                  xpos + w, ypos + h, 1.0f, 0.0f;

      ch->texture->bind();
      text_quad_buf_.replace(vertices);      
      text_quad_buf_.bind();

      glDrawArrays(GL_TRIANGLES, 0, 6);

      ch->texture->unbind();
      text_quad_buf_.unbind();
      
      // Advance is stored in number of 1/64 pixels
      x += (ch->advance >> 6) * text_scale_x_;
    }
  }

  for (auto& tick : y_ticks_) {
    float y = tick;
    float x = 0.0;
    std::string tick_s = get_tick_string(tick);
    std::string::const_iterator c;

    for (c = tick_s.begin(); c != tick_s.end(); c++) {
      std::shared_ptr<Character> ch = font_.get_char(*c);

      float xpos = x + ch->bearing[0] * text_scale_x_;
      float ypos = y - (ch->size[1] - ch->bearing[1]) * text_scale_y_;
      float w = ch->size[0] * text_scale_x_;
      float h = ch->size[1] * text_scale_y_;

      MatrixX4f vertices(6, 4);
      vertices << xpos, ypos + h,     0.0f, 0.0f,
                  xpos, ypos,         0.0f, 1.0f,
                  xpos + w, ypos,     1.0f, 1.0f,
                  xpos, ypos + h,     0.0f, 0.0f,
                  xpos + w, ypos,     1.0f, 1.0f,
                  xpos + w, ypos + h, 1.0f, 0.0f;
      ch->texture->bind();
      text_quad_buf_.replace(vertices);      
      text_quad_buf_.bind();

      glDrawArrays(GL_TRIANGLES, 0, 6);
      
      ch->texture->unbind();
      text_quad_buf_.unbind();

      // Advance is stored in number of 1/64 pixels
      x += (ch->advance >> 6) * text_scale_x_;
    }
  }

  glDisable(GL_BLEND);
}

void Axes::update_limits(float x_min, float x_max, float y_min, float y_max, int window_width, int window_height) {
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

  update_scale(window_width, window_height);

  if (auto_ticks_)
    update_ticks();
}

void Axes::update_scale(int width, int height) {
  text_scale_x_ = 1.0 * (x_max_ - x_min_) / (float)width;
  text_scale_y_ = 1.0 * (y_max_ - y_min_) / (float)height;
}

void Axes::update_ticks() {
  Vector2f p = get_scaled_padding();
  float x_delta = (2.0 * p[0] + x_max_ - x_min_) / 9.0f;
  float y_delta = (2.0 * p[1] + y_max_ - y_min_) / 9.0f;

  x_ticks_.clear();
  y_ticks_.clear();

  int i = 0;
  while (i * x_delta < x_max_) {
    i++;
    if (x_delta > 1.0)
      x_ticks_.push_back(std::ceil(i * x_delta));
    else
      x_ticks_.push_back(i * x_delta);
  }

  i = 0;
  while (-i * x_delta > x_min_) {
    i++;
    if (x_delta > 1.0)
      x_ticks_.push_back(std::ceil(-i * x_delta));
    else
      x_ticks_.push_back(-i * x_delta);
  }

  i = 0;
  while (i * y_delta < y_max_) {
    i++;
    if (y_delta > 1.0)
      y_ticks_.push_back(std::ceil(i * y_delta));
    else
      y_ticks_.push_back(i * y_delta);
  }

  i = 0;
  while (-i * y_delta > y_min_) {
    i++;
    if (y_delta > 1.0)
      y_ticks_.push_back(std::ceil(-i * y_delta));
    else
      y_ticks_.push_back(-i * y_delta);
  }

}

void Axes::set_xticks(std::vector<float> ticks) {
  auto_ticks_ = false;
  x_ticks_ = ticks;
}

void Axes::set_yticks(std::vector<float> ticks) {
  auto_ticks_ = false;
  y_ticks_ = ticks;
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

std::string Axes::get_tick_string(float tick) {
  std::ostringstream out;
  out.precision(2);
  out << tick;
  return out.str();
}
