#ifndef CANNON_GRAPHICS_FRAMEBUFFER_H
#define CANNON_GRAPHICS_FRAMEBUFFER_H 

#include <stdexcept>
#include <memory>
#include <queue>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cannon/graphics/texture.hpp>
#include <cannon/graphics/shader_program.hpp>
#include <cannon/graphics/vertex_array_object.hpp>
#include <cannon/graphics/vertex_buffer.hpp>
#include <cannon/graphics/geometry/screen_quad.hpp>

namespace cannon {
  namespace graphics {

    class Framebuffer {
      public:
        Framebuffer(int width=800, int height=600, const std::string& name="") :
          width(width), height(height), name(name) {

          glGenFramebuffers(1, &gl_framebuffer_);
          glBindFramebuffer(GL_FRAMEBUFFER, gl_framebuffer_);

          auto color_buf = std::make_shared<Texture>(width, height, GL_RGBA16F, GL_FLOAT);
          attach_tex(color_buf);
          
          quad = std::make_shared<geometry::ScreenQuad>(color_buf, width, height);
          generate_depth_stencil_buffer_();

          unbind();
        }

        Framebuffer(std::vector<std::shared_ptr<Texture>> attachments, int
            width=800, int height=600, const std::string& name="") :
          width(width), height(height), name(name) {

          glGenFramebuffers(1, &gl_framebuffer_);
          glBindFramebuffer(GL_FRAMEBUFFER, gl_framebuffer_);

          for (auto& tex : attachments)
            attach_tex(tex);
          
          quad = std::make_shared<geometry::ScreenQuad>(color_attachments[0], width, height);
          generate_depth_stencil_buffer_();

          unbind();
        }

        ~Framebuffer() {
          if (glfwGetCurrentContext() != NULL) {
            glDeleteFramebuffers(1, &gl_framebuffer_);
          }
        }

        // Sets GL_READ_FRAMEBUFFER to gl_framebuffer_, and GL_DRAW_FRAMEBUFFER
        // to gl_framebuffer_
        void bind(); 

        // Sets GL_READ_FRAMEBUFFER to gl_framebuffer_
        void bind_read();

        // Sets GL_READ_FRAMEBUFFER to 0
        void unbind_read();

        // Sets GL_DRAW_FRAMEBUFFER to gl_framebuffer_
        void bind_draw();

        // Sets GL_DRAW_FRAMEBUFFER to 0
        void unbind_draw();

        // Sets GL_READ_FRAMEBUFFER to 0 and GL_DRAW_FRAMEBUFFER to 0
        void unbind();

        // Sets GL_READ_FRAMEBUFFER to gl_framebuffer_ and GL_DRAW_FRAMEBUFFER
        // to gl_framebuffer_
        void display();

        // Does not affect OpenGL state
        void draw_quad();

        // Does not affect OpenGL state
        void draw_quad(int idx);

        // Does not affect OpenGL state
        void attach_tex(std::shared_ptr<Texture> tex);

        // Does not affect OpenGL state
        void resize(int w, int h);
        
        // Does not affect OpenGL state
        void write_imgui();

        int width;
        int height;

        std::shared_ptr<geometry::ScreenQuad> quad;
        std::string name;
        std::deque<std::shared_ptr<Texture>> color_attachments;

      private:
        void generate_depth_stencil_buffer_();

        unsigned int gl_framebuffer_;
        unsigned int gl_depth_stencil_rb_;

        std::shared_ptr<Framebuffer> screen_fb_;

        unsigned int max_color_attachments_ = 8;

    };

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_FRAMEBUFFER_H */
