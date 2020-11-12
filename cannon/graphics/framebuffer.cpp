#include <cannon/graphics/framebuffer.hpp>

using namespace cannon::graphics;

void Framebuffer::bind() {
  glBindFramebuffer(GL_FRAMEBUFFER, gl_framebuffer_);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    throw std::runtime_error("Bound framebuffer is not complete");
}

void Framebuffer::unbind() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::display() {
  unbind();
  draw_color_buffer_quad_();
  bind();
}

void Framebuffer::resize(int w, int h) {
  width = w;
  height = h;

  color_buffer_ = std::make_shared<Texture>(width, height);
  color_buffer_->bind();
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
      GL_TEXTURE_2D, color_buffer_->gl_texture_, 0);

  glDeleteRenderbuffers(1, &gl_depth_stencil_rb_);
  generate_depth_stencil_buffer_();
}

void Framebuffer::generate_depth_stencil_buffer_() {
  glGenRenderbuffers(1, &gl_depth_stencil_rb_);
  glBindRenderbuffer(GL_RENDERBUFFER, gl_depth_stencil_rb_);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER,
      GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
      gl_depth_stencil_rb_);
}

void Framebuffer::populate_quad_buf_() {
  vertices_ << -1.0f,  1.0f,
               -1.0f, -1.0f,
                1.0f, -1.0f,
                
               -1.0f,  1.0f,
                1.0f, -1.0f,
                1.0f,  1.0f;

  buf_.buffer(vertices_);

  texture_coords_ << 0.0f, 1.0f,
                     0.0f, 0.0f,
                     1.0f, 0.0f,
                     
                     0.0f, 1.0f,
                     1.0f, 0.0f,
                     1.0f, 1.0f;

  texture_coord_buf_.buffer(texture_coords_);
}

void Framebuffer::draw_color_buffer_quad_() {
  quad_program->activate();
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  color_buffer_->bind();
  buf_.bind();
  texture_coord_buf_.bind();

  glDisable(GL_DEPTH_TEST);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glEnable(GL_DEPTH_TEST);
}
