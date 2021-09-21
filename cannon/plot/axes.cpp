#include <cannon/plot/axes.hpp>

#include <cmath>
#include <sstream>

#include <imgui.h>

#include <cannon/graphics/texture.hpp>
#include <cannon/graphics/character.hpp>
#include <cannon/graphics/vertex_array_object.hpp>
#include <cannon/graphics/vertex_shader.hpp>
#include <cannon/graphics/fragment_shader.hpp>
#include <cannon/graphics/shader_program.hpp>
#include <cannon/graphics/vertex_buffer.hpp>
#include <cannon/graphics/font.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::plot;
using namespace cannon::log;
using namespace cannon::graphics;

Axes::Axes(float text_scale, bool outside)
    : x_min_(-0.01), x_max_(0.01), y_min_(-0.01), y_max_(0.01),
      text_scale_x_(0.1), text_scale_y_(0.1), outside_(outside), padding_(0.1),
      text_scale_multiplier_(text_scale), program_(new ShaderProgram),
      vao_(new VertexArrayObject), buf_(new VertexBuffer(vao_)),
      font_(new Font(true, 12)), text_program_(new ShaderProgram),
      text_vao_(new VertexArrayObject),
      text_quad_buf_(new VertexBuffer(text_vao_)) {

  MatrixX2f lines(4, 2);

  if (outside_) {
    lines << x_min_, y_min_,
             x_max_, y_min_,
             x_min_, y_min_, 
             x_min_, y_max_; 
  } else {
    lines << x_min_, 0.0,
             x_max_, 0.0,
             0.0, y_min_, 
             0.0, y_max_; 
  }

  buf_->buffer(lines);
  log_info(buf_);

  x_ticks_.push_back(-1.0);
  x_ticks_.push_back(1.0);
  y_ticks_.push_back(-1.0);
  y_ticks_.push_back(1.0);

  const char *v_src = BASIC_VERTEX_SHADER_2D.c_str();
  const char *f_src = BASIC_FRAGMENT_SHADER.c_str();
  VertexShader v(&v_src);
  FragmentShader f(&f_src);

  program_->attach_shader(v);
  program_->attach_shader(f);
  program_->link();

  VertexShader tv = load_vertex_shader("shaders/char_shader.vert");
  FragmentShader tf = load_fragment_shader("shaders/char_shader.frag");

  text_program_->attach_shader(tv);
  text_program_->attach_shader(tf);
  text_program_->link();
  
  MatrixX4f vertices(MatrixX4f::Zero(6, 4));
  text_quad_buf_->buffer(vertices);
}

void Axes::draw() {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glLineWidth(axis_line_width_);

  program_->set_uniform("uColor", Vector4f(0.0, 0.0, 0.0, 1.0));
  program_->set_uniform("matrix", make_scaling_matrix());
  program_->activate();
  buf_->bind();
  
  glDrawArrays(GL_LINES, 0, 4);

  glLineWidth(1.0);

  text_program_->set_uniform("textColor", Vector4f(0.0, 0.0, 0.0, 1.0));
  text_program_->set_uniform("projection", make_scaling_matrix());
  text_program_->set_uniform("text", 0);
  text_program_->activate();

  for (auto& tick : x_ticks_) {
    float x = tick;
    float y = outside_ ? y_min_ : 0.0;
    y += text_offset_y_;

    std::string tick_s = get_tick_string(tick);
    std::string::const_iterator c;

    for (c = tick_s.begin(); c != tick_s.end(); c++) {
      std::shared_ptr<Character> ch = font_->get_char(*c);

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
      text_quad_buf_->replace(vertices);      
      text_quad_buf_->bind();

      glDrawArrays(GL_TRIANGLES, 0, 6);

      ch->texture->unbind();
      text_quad_buf_->unbind();
      
      // Advance is stored in number of 1/64 pixels
      x += (ch->advance >> 6) * text_scale_x_;
    }
  }

  for (auto& tick : y_ticks_) {
    float y = tick;
    float x = outside_ ? x_min_ : 0.0;
    x += text_offset_x_;

    std::string tick_s = get_tick_string(tick);
    std::string::const_iterator c;

    for (c = tick_s.begin(); c != tick_s.end(); c++) {
      std::shared_ptr<Character> ch = font_->get_char(*c);

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
      text_quad_buf_->replace(vertices);      
      text_quad_buf_->bind();

      glDrawArrays(GL_TRIANGLES, 0, 6);
      
      ch->texture->unbind();
      text_quad_buf_->unbind();

      // Advance is stored in number of 1/64 pixels
      x += (ch->advance >> 6) * text_scale_x_;
    }
  }

  glDisable(GL_BLEND);
}

void Axes::update_limits() {
  Vector2f scaled_padding = get_scaled_padding();

  MatrixX2f lines(4, 2);

  if (outside_) {
    lines << x_min_, y_min_,
             x_max_, y_min_,
             x_min_, y_min_, 
             x_min_, y_max_; 
  } else {
    lines << x_min_ - scaled_padding[0], 0.0,
             x_max_ + scaled_padding[0], 0.0,
             0.0, y_min_ - scaled_padding[1], 
             0.0, y_max_ + scaled_padding[1]; 
  }
  buf_->buffer(lines);

  if (auto_ticks_)
    update_ticks();
}

void Axes::update_limits(float x_min, float x_max, float y_min, float y_max, int window_width, int window_height) {
  x_min_ = std::min(x_min, x_min_);
  y_min_ = std::min(y_min, y_min_);
  x_max_ = std::max(x_max, x_max_);
  y_max_ = std::max(y_max, y_max_);

  update_limits();

  update_scale(window_width, window_height);
}

void Axes::update_scale(int width, int height) {
  text_scale_x_ = text_scale_multiplier_ * (x_max_ - x_min_) / (float)width;
  text_scale_y_ = text_scale_multiplier_ * (y_max_ - y_min_) / (float)height;
}

void Axes::update_ticks() {
  Vector2f p = get_scaled_padding();
  float x_delta = (2.0 * p[0] + x_max_ - x_min_) / 9.0f;
  float y_delta = (2.0 * p[1] + y_max_ - y_min_) / 9.0f;

  x_ticks_.clear();
  y_ticks_.clear();

  if (outside_) {
    if (x_min_ < 0.0 && 0.0 < x_max_) {
      x_ticks_.push_back(0.0);
    }

    if (y_min_ < 0.0 && 0.0 < y_max_) {
      y_ticks_.push_back(0.0);
    }
  } 

  int i = 0;
  while (i * x_delta < x_max_) {
    i++;
    x_ticks_.push_back(i * x_delta);
  }

  i = 0;
  while (-i * x_delta > x_min_) {
    i++;
    x_ticks_.push_back(-i * x_delta);
  }

  i = 0;
  while (i * y_delta < y_max_) {
    i++;
    y_ticks_.push_back(i * y_delta);
  }

  i = 0;
  while (-i * y_delta > y_min_) {
    i++;
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

void Axes::write_imgui() {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("Plotting")) {
      ImGui::SliderFloat("Text Scale", &text_scale_multiplier_, 0.1, 5.0);
      ImGui::SliderFloat("Padding", &padding_, 0.01, 1.0);

      ImGui::SliderFloat("X Ticks Offset", &text_offset_y_, -1.0, 1.0);
      ImGui::SliderFloat("Y Ticks Offset", &text_offset_x_, -1.0, 1.0);
      ImGui::SliderFloat("Axis Width", &axis_line_width_, 0.1, 10.0);

      bool changed = false;

      changed = changed || ImGui::InputFloat("X Min", &x_min_);
      changed = changed || ImGui::InputFloat("X Max", &x_max_);
      changed = changed || ImGui::InputFloat("Y Min", &y_min_);
      changed = changed || ImGui::InputFloat("Y Max", &y_max_);
      changed = changed || ImGui::Checkbox("Axes Outside", &outside_);

      if (changed)
        update_limits();

      ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
  }
}

Matrix4f Axes::make_scaling_matrix() {
  // TODO Extract as Layout system
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

