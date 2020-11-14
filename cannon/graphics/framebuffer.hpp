#ifndef CANNON_GRAPHICS_FRAMEBUFFER_H
#define CANNON_GRAPHICS_FRAMEBUFFER_H 

#include <stdexcept>
#include <memory>

#include <glad/glad.h>

#include <cannon/graphics/texture.hpp>
#include <cannon/graphics/shader_program.hpp>
#include <cannon/graphics/vertex_array_object.hpp>
#include <cannon/graphics/vertex_buffer.hpp>
#include <cannon/graphics/geometry/screen_quad.hpp>

namespace cannon {
  namespace graphics {

    class Framebuffer {
      public:
        Framebuffer(int width=800, int height=600, bool msaa=true) :
          width(width), height(height), msaa_(msaa) {

          glGenFramebuffers(1, &gl_framebuffer_);
          glBindFramebuffer(GL_FRAMEBUFFER, gl_framebuffer_);

          if (msaa_) {
            color_buffer_ = std::make_shared<Texture>(width, height, true);
            color_buffer_->bind();
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                GL_TEXTURE_2D_MULTISAMPLE, color_buffer_->gl_texture_, 0);

            screen_fb_ = std::make_shared<Framebuffer>(width, height, false);
            screen_fb_->bind();
            screen_fb_->unbind();

            quad = screen_fb_->quad;
          } else {
            color_buffer_ = std::make_shared<Texture>(width, height, false);
            color_buffer_->bind();
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                GL_TEXTURE_2D, color_buffer_->gl_texture_, 0);

            quad = std::make_shared<geometry::ScreenQuad>(color_buffer_, width, height);
          }
          
          // We don't currently have any need to encapsulate this

          generate_depth_stencil_buffer_();
        }

        ~Framebuffer() {
          glDeleteFramebuffers(1, &gl_framebuffer_);
        }

        // Bind makes the framebuffer receive draw commands. Unbind is
        // necessary before the default framebuffer will display changes again.
        // Display draws this framebuffer on the currently bound framebuffer.
        void bind(); 
        void unbind();
        void display();

        void resize(int w, int h);

        int width;
        int height;

        std::shared_ptr<geometry::ScreenQuad> quad;

      private:
        void draw_color_buffer_quad_();
        void generate_depth_stencil_buffer_();

        unsigned int gl_framebuffer_;
        unsigned int gl_depth_stencil_rb_;

        std::shared_ptr<Texture> color_buffer_;
        std::shared_ptr<Framebuffer> screen_fb_;

        bool msaa_;

    };

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_FRAMEBUFFER_H */
