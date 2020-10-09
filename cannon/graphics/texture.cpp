#include <cannon/graphics/texture.hpp>

using namespace cannon::graphics;

void Texture::bind() {
  glActiveTexture(gl_texture_unit_);
  glBindTexture(GL_TEXTURE_2D, gl_texture_);
}

void Texture::bind(GLenum texture_unit) {
  // texture_unit should be GL_TEXTURE0 - GL_TEXTURE15
  glActiveTexture(texture_unit);
  glBindTexture(GL_TEXTURE_2D, gl_texture_);
}

void Texture::unbind() {
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::set_wrap_repeat() {
  bind();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void Texture::set_wrap_mirrored_repeat() {
  bind();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
}

void Texture::set_wrap_clamp_edge() {
  bind();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Texture::set_wrap_clamp_border() {
  bind();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
}

void Texture::set_filter_linear() {
  bind();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::set_filter_nearest() {
  bind();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}
