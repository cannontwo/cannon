#include <cannon/graphics/texture.hpp>

#include <imgui.h>

using namespace cannon::graphics;

void Texture::bind() const {
  glActiveTexture(gl_texture_unit_);
  glBindTexture(GL_TEXTURE_2D, gl_texture_);
}


void Texture::bind(GLenum texture_unit) const {
  // texture_unit should be GL_TEXTURE0 - GL_TEXTURE15
  glActiveTexture(texture_unit);
  glBindTexture(GL_TEXTURE_2D, gl_texture_);
}

void Texture::unbind() const {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::unbind(GLenum texture_unit) const {
  glActiveTexture(texture_unit);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::framebuffer_bind(GLenum attachment) const {
  glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, gl_texture_, 0);
}

void Texture::set_wrap_repeat() {
  bind();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  unbind();
}

void Texture::set_wrap_mirrored_repeat() {
  bind();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  unbind();
}

void Texture::set_wrap_clamp_edge() {
  bind();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  unbind();
}

void Texture::set_wrap_clamp_border() {
  bind();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  unbind();
}

void Texture::set_filter_linear() {
  bind();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  unbind();
}

void Texture::set_filter_nearest() {
  bind();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  unbind();
}

void Texture::write_imgui() {
  ImTextureID tex_id = (void *)(intptr_t)gl_texture_;

  if (path.empty()) {
    // Rendering is flipped in ImGui
    static float texture_preview_size = 100.0;
    ImGui::SliderFloat("Preview Size", &texture_preview_size, 100.0, 500.0);
    ImGui::Image(tex_id, ImVec2(texture_preview_size, texture_preview_size),
        ImVec2(0.0, 1.0), ImVec2(1.0, 0.0));
  } else {
    if (ImGui::TreeNode(path.c_str())) {
      static float texture_preview_size = 100.0;
      ImGui::SliderFloat("Preview Size", &texture_preview_size, 100.0, 500.0);
      ImGui::Image(tex_id, ImVec2(texture_preview_size, texture_preview_size),
          ImVec2(0.0, 1.0), ImVec2(1.0, 0.0));
      ImGui::TreePop();
    }
  }
}

void Texture::buffer() {
  bind();
  glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width_, height_, 0, format_, data_type, data_);
  unbind();
}
