#include <cannon/graphics/framebuffer.hpp>

#include <imgui.h>

#include <cannon/graphics/texture.hpp>
#include <cannon/graphics/geometry/screen_quad.hpp>

using namespace cannon::graphics;

Framebuffer::Framebuffer(int width, int height, const std::string& name) :
  width(width), height(height), name(name) {

  glGenFramebuffers(1, &gl_framebuffer_);
  glBindFramebuffer(GL_FRAMEBUFFER, gl_framebuffer_);

  auto color_buf = std::make_shared<Texture>(width, height, GL_RGBA16F, GL_FLOAT);
  attach_tex(color_buf);
  
  quad = std::make_shared<geometry::ScreenQuad>(color_buf, width, height);
  generate_depth_stencil_buffer_();

  unbind();
}

Framebuffer::Framebuffer(std::vector<TexturePtr> attachments, int width, int
    height, const std::string& name) : width(width), height(height), name(name)
{
  glGenFramebuffers(1, &gl_framebuffer_);
  glBindFramebuffer(GL_FRAMEBUFFER, gl_framebuffer_);

  for (auto& tex : attachments)
    attach_tex(tex);
  
  quad = std::make_shared<geometry::ScreenQuad>(color_attachments[0], width, height);
  generate_depth_stencil_buffer_();

  unbind();
}

void Framebuffer::bind() {
  glBindFramebuffer(GL_FRAMEBUFFER, gl_framebuffer_);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    throw std::runtime_error("Bound framebuffer is not complete");

  std::vector<unsigned int> attachments;
  for (unsigned int i = 0; i < color_attachments.size(); i++)
    attachments.push_back(GL_COLOR_ATTACHMENT0 + i);

  glDrawBuffers(color_attachments.size(), attachments.data());
}

void Framebuffer::bind_read() {
  glBindFramebuffer(GL_READ_FRAMEBUFFER, gl_framebuffer_);
}

void Framebuffer::unbind_read() {
  glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}

void Framebuffer::bind_draw() {
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gl_framebuffer_);
}

void Framebuffer::unbind_draw() {
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void Framebuffer::unbind() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::display() {
  unbind();
  glEnable(GL_FRAMEBUFFER_SRGB); 
  draw_quad();
  glDisable(GL_FRAMEBUFFER_SRGB);
  bind();
}

void Framebuffer::attach_tex(std::shared_ptr<Texture> tex) {
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
    bind();

  color_attachments.push_back(tex);

  if (color_attachments.size() > max_color_attachments_) {
    color_attachments.pop_front();

    for (unsigned int i = 0; i < color_attachments.size(); i++) {
      color_attachments[i]->bind();
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i,
          GL_TEXTURE_2D, color_attachments[i]->gl_texture_, 0);
    }
  } else {
    tex->bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0+(color_attachments.size() - 1), GL_TEXTURE_2D,
        tex->gl_texture_, 0);
    tex->unbind();
  }
  unbind();
}

void Framebuffer::resize(int w, int h) {
  width = w;
  height = h;

  bind();

  for (unsigned int i = 0; i < color_attachments.size(); i++) {
    GLint internal_format = color_attachments[i]->internal_format;
    GLenum type = color_attachments[i]->data_type;
    color_attachments[i] = std::make_shared<Texture>(width, height, internal_format, type);
    color_attachments[i]->bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i,
        GL_TEXTURE_2D, color_attachments[i]->gl_texture_, 0);
    color_attachments[i]->unbind();
  }

  quad->resize(w, h);
  quad->set_tex(color_attachments[0]);

  glDeleteRenderbuffers(1, &gl_depth_stencil_rb_);
  generate_depth_stencil_buffer_();
  unbind();
}

void Framebuffer::write_imgui() {
  if (ImGui::TreeNode(name.c_str())) {
    ImGui::Text("Width: %d", width);
    ImGui::SameLine();
    ImGui::Text("Height: %d", height);

    for (auto& color_buffer : color_attachments)
      color_buffer->write_imgui();

    ImGui::TreePop();
  }
}

void Framebuffer::generate_depth_stencil_buffer_() {
  bind();

  glGenRenderbuffers(1, &gl_depth_stencil_rb_);
  glBindRenderbuffer(GL_RENDERBUFFER, gl_depth_stencil_rb_);

  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

  glFramebufferRenderbuffer(GL_FRAMEBUFFER,
      GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
      gl_depth_stencil_rb_);
  unbind();
}

void Framebuffer::draw_quad() {
  draw_quad(0);
}

void Framebuffer::draw_quad(int idx) {
  quad->set_tex(color_attachments[idx]);
  quad->draw();
}
