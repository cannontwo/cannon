#include <cannon/graphics/framebuffer.hpp>

using namespace cannon::graphics;

void Framebuffer::bind() {
  glBindFramebuffer(GL_FRAMEBUFFER, gl_framebuffer_);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    throw std::runtime_error("Bound framebuffer is not complete");
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
  draw_color_buffer_quad();
  bind();
}

void Framebuffer::resize(int w, int h) {
  width = w;
  height = h;

  bind();
  if (msaa_) {
    color_buffer = std::make_shared<Texture>(width, height, true);
    color_buffer->bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D_MULTISAMPLE, color_buffer->gl_texture_, 0);

    screen_fb_->resize(w, h);
    bind();
  } else {
    color_buffer = std::make_shared<Texture>(width, height, false);
    color_buffer->bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D, color_buffer->gl_texture_, 0);

    quad->resize(w, h);
    quad->set_tex(color_buffer);
  }

  glDeleteRenderbuffers(1, &gl_depth_stencil_rb_);
  generate_depth_stencil_buffer_();
}

void Framebuffer::write_imgui() {
  if (ImGui::TreeNode(name.c_str())) {
    ImGui::Text("Width: %d", width);
    ImGui::SameLine();
    ImGui::Text("Height: %d", height);
    ImGui::Text("MSAA: %d", msaa_);

    if (msaa_) 
      screen_fb_->color_buffer->write_imgui();
    else
      color_buffer->write_imgui();

    ImGui::TreePop();
  }

}

void Framebuffer::generate_depth_stencil_buffer_() {
  bind();
  glGenRenderbuffers(1, &gl_depth_stencil_rb_);
  glBindRenderbuffer(GL_RENDERBUFFER, gl_depth_stencil_rb_);

  if (msaa_)
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 16, GL_DEPTH24_STENCIL8, width, height);
  else
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

  glFramebufferRenderbuffer(GL_FRAMEBUFFER,
      GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
      gl_depth_stencil_rb_);
}

void Framebuffer::draw_color_buffer_quad() {
  if (msaa_) {
    // Blitting to secondary framebuffer necessary to actually get MSAA behavior
    bind();
    glBindFramebuffer(GL_READ_FRAMEBUFFER, gl_framebuffer_);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, screen_fb_->gl_framebuffer_);
    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    unbind();

    screen_fb_->draw_color_buffer_quad();
  } else {
    quad->draw();
  }
}
