#ifndef CANNON_GRAPHICS_FRAMEBUFFER_H
#define CANNON_GRAPHICS_FRAMEBUFFER_H 

#include <stdexcept>
#include <memory>
#include <queue>

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
        Framebuffer(int width=800, int height=600, const std::string& name="") :
          width(width), height(height), name(name) {

          glGenFramebuffers(1, &gl_framebuffer_);
          glBindFramebuffer(GL_FRAMEBUFFER, gl_framebuffer_);

          auto color_buf = std::make_shared<Texture>(width, height);
          attach_tex(color_buf);
          
          quad = std::make_shared<geometry::ScreenQuad>(color_buf, width, height);
          generate_depth_stencil_buffer_();
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
        }

        ~Framebuffer() {
          glDeleteFramebuffers(1, &gl_framebuffer_);
        }

        // Bind makes the framebuffer receive draw commands. Unbind is
        // necessary before the default framebuffer will display changes again.
        // Display draws this framebuffer on the currently bound framebuffer.
        void bind(); 
        void bind_read();
        void unbind_read();
        void bind_draw();
        void unbind_draw();
        void unbind();
        void display();

        void draw_quad();
        void draw_quad(int idx);

        void attach_tex(std::shared_ptr<Texture> tex);

        void resize(int w, int h);

        int width;
        int height;
        
        void write_imgui();

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
