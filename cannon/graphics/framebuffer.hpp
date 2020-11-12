#ifndef CANNON_GRAPHICS_FRAMEBUFFER_H
#define CANNON_GRAPHICS_FRAMEBUFFER_H 

#include <stdexcept>
#include <memory>

#include <glad/glad.h>

#include <cannon/graphics/texture.hpp>
#include <cannon/graphics/shader_program.hpp>
#include <cannon/graphics/vertex_array_object.hpp>
#include <cannon/graphics/vertex_buffer.hpp>

namespace cannon {
  namespace graphics {

    class Framebuffer {
      public:
        Framebuffer(int width=800, int height=600) : width(width),
        height(height), vao_(new VertexArrayObject), buf_(vao_),
        texture_coord_buf_(vao_), vertices_(6, 2), texture_coords_(6, 2) {

          glGenFramebuffers(1, &gl_framebuffer_);
          glBindFramebuffer(GL_FRAMEBUFFER, gl_framebuffer_);

          color_buffer_ = std::make_shared<Texture>(width, height);
          color_buffer_->bind();
          glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
              GL_TEXTURE_2D, color_buffer_->gl_texture_, 0);
          
          // We don't currently have any need to encapsulate this

          generate_depth_stencil_buffer_();

          quad_program = std::make_shared<ShaderProgram>("quad_shader");
          quad_program->attach_vertex_shader("shaders/pass_pos_tex.vert");
          quad_program->attach_fragment_shader("shaders/tex_quad.frag");
          quad_program->link();

          populate_quad_buf_();

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

        std::shared_ptr<ShaderProgram> quad_program;

      private:
        void draw_color_buffer_quad_();
        void populate_quad_buf_();
        void generate_depth_stencil_buffer_();

        unsigned int gl_framebuffer_;
        unsigned int gl_depth_stencil_rb_;

        std::shared_ptr<Texture> color_buffer_;

        std::shared_ptr<VertexArrayObject> vao_;
        VertexBuffer buf_;
        VertexBuffer texture_coord_buf_;

        MatrixX2f vertices_;
        MatrixX2f texture_coords_;

    };

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_FRAMEBUFFER_H */
