#include <cannon/graphics/text_renderer.hpp>

#include <cannon/graphics/font.hpp>
#include <cannon/graphics/character.hpp>
#include <cannon/graphics/texture.hpp>
#include <cannon/graphics/projection.hpp>
#include <cannon/graphics/vertex_array_object.hpp>
#include <cannon/graphics/vertex_buffer.hpp>
#include <cannon/graphics/shader_program.hpp>
#include <cannon/graphics/vertex_shader.hpp>
#include <cannon/graphics/fragment_shader.hpp>

using namespace cannon::graphics;

TextRenderer::TextRenderer(unsigned int width, unsigned int height,
                           FontPtr font)
    : width_(width), height_(height), font_(font), vao_(new VertexArrayObject),
      buf_(new VertexBuffer(vao_)),
      program_(new ShaderProgram("text_program")),
      text_color_(Vector4f(0.0, 0.0, 0.0, 1.0)) {

  font_->init();

  // Compile shader
  VertexShader v = load_vertex_shader("shaders/char_shader.vert");
  FragmentShader f = load_fragment_shader("shaders/char_shader.frag");

  program_->attach_shader(v);
  program_->attach_shader(f);
  program_->link();

  // Reserve space for display quads
  MatrixX4f vertices(MatrixX4f::Zero(6, 4));
  buf_->bind();
  buf_->buffer(vertices);
}

void TextRenderer::draw(const std::string &text, double start_x, double start_y,
                        double scale) {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  vao_->bind();
  program_->set_uniform(
      "projection", make_orthographic(0.0, static_cast<float>(width_), 0.0,
                                      static_cast<float>(height_), -1.0, 1.0));
  program_->set_uniform("textColor", text_color_);
  program_->set_uniform("text", 0);
  program_->activate();

  float x = start_x;
  float y = start_y;

  std::string::const_iterator c;   

  for (c = text.begin(); c != text.end(); c++) {
    std::shared_ptr<Character> ch = font_->get_char(*c);

    float xpos = x + ch->bearing[0] * scale;
    float ypos = y - (ch->size[1] - ch->bearing[1]) * scale;
    float w = ch->size[0] * scale;
    float h = ch->size[1] * scale;

    MatrixX4f vertices(6, 4);
    vertices << xpos, ypos + h,     0.0f, 0.0f,
                xpos, ypos,         0.0f, 1.0f,
                xpos + w, ypos,     1.0f, 1.0f,
                xpos, ypos + h,     0.0f, 0.0f,
                xpos + w, ypos,     1.0f, 1.0f,
                xpos + w, ypos + h, 1.0f, 0.0f;
    ch->texture->bind();
    buf_->replace(vertices);      
    buf_->bind();

    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    // Advance is stored in number of 1/64 pixels
    x += (ch->advance >> 6) * scale;
  }

  glDisable(GL_BLEND);
}

Vector4f& TextRenderer::text_color() {
  return text_color_;
}

unsigned int& TextRenderer::width() {
  return width_;
}

unsigned int& TextRenderer::height() {
  return height_;
}
