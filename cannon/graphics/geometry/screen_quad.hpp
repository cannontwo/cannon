#ifndef CANNON_GRAPHICS_GEOMETRY_SCREEN_QUAD_H
#define CANNON_GRAPHICS_GEOMETRY_SCREEN_QUAD_H 

#include <cannon/graphics/vertex_array_object.hpp>
#include <cannon/graphics/vertex_buffer.hpp>
#include <cannon/graphics/shader_program.hpp>
#include <cannon/graphics/texture.hpp>

namespace cannon {
  namespace graphics {
    namespace geometry {

      class ScreenQuad {
        public:
          ScreenQuad() = delete;

          ScreenQuad(std::shared_ptr<Texture> tex, int width=800, int
              height=600) : width(width), height(height), tex_(tex) , vao_(new
                VertexArrayObject), buf_(vao_), texture_coord_buf_(vao_),
              vertices_(6, 2), texture_coords_(6, 2) {

            program = std::make_shared<ShaderProgram>("screen_quad_shader");
            program->attach_vertex_shader("shaders/pass_pos_tex.vert");
            program->attach_fragment_shader("shaders/tex_quad.frag");
            program->link();

            populate_quad_buf_();

          }

          void resize(int w, int h);

          void draw();
          void draw(std::shared_ptr<ShaderProgram> p);

          void set_tex(std::shared_ptr<Texture> t);

          int width;
          int height;

          std::shared_ptr<ShaderProgram> program;

        private:
          void populate_quad_buf_();

          std::shared_ptr<Texture> tex_;
          std::shared_ptr<VertexArrayObject> vao_;

          VertexBuffer buf_;
          VertexBuffer texture_coord_buf_;

          MatrixX2f vertices_;
          MatrixX2f texture_coords_;

      };

    } // namespace geometry
  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_GEOMETRY_SCREEN_QUAD_H */
