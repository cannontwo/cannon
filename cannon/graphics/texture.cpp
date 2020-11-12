#include <cannon/graphics/texture.hpp>

using namespace cannon::graphics;

void Texture::bind() const {
  glActiveTexture(gl_texture_unit_);
  if (msaa_)
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, gl_texture_);
  else
    glBindTexture(GL_TEXTURE_2D, gl_texture_);
}


void Texture::bind(GLenum texture_unit) const {
  // texture_unit should be GL_TEXTURE0 - GL_TEXTURE15
  glActiveTexture(texture_unit);
  if (msaa_)
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, gl_texture_);
  else
    glBindTexture(GL_TEXTURE_2D, gl_texture_);
}

void Texture::unbind() const {
  if (msaa_)
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
  else
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::set_wrap_repeat() {
  bind();
  if (msaa_) {
    // No-op on multi-sampled textures
  } else {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  }
}

void Texture::set_wrap_mirrored_repeat() {
  bind();
  if (msaa_) {
    // No-op on multi-sampled textures
  } else {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  }
}

void Texture::set_wrap_clamp_edge() {
  bind();
  if (msaa_) {
    // No-op on multi-sampled textures
  } else {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }
}

void Texture::set_wrap_clamp_border() {
  bind();
  if (msaa_) {
    // No-op on multi-sampled textures
  } else {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  }
}

void Texture::set_filter_linear() {
  bind();
  if (msaa_) {
    // No-op on multi-sampled textures
  } else {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }
}

void Texture::set_filter_nearest() {
  bind();
  if (msaa_) {
    // No-op on multi-sampled textures
  } else {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  }
}
