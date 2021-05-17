#ifndef CANNON_GRAPHICS_GEOMETRY_SCREEN_QUAD_H
#define CANNON_GRAPHICS_GEOMETRY_SCREEN_QUAD_H 

#include <cannon/graphics/vertex_buffer.hpp>
#include <cannon/utils/class_forward.hpp>

namespace cannon {
  namespace graphics {

    CANNON_CLASS_FORWARD(Texture);
    CANNON_CLASS_FORWARD(VertexArrayObject);
    CANNON_CLASS_FORWARD(ShaderProgram);

    namespace geometry {

      class ScreenQuad {
        public:
          ScreenQuad() = delete;

          // Does not affect OpenGL state
          ScreenQuad(std::shared_ptr<Texture> tex, int width=800, int
              height=600);

          // Does not affect OpenGL state
          void resize(int w, int h);

          // Does not affect OpenGL state
          void draw();

          // Does not affect OpenGL state
          void draw(ShaderProgramPtr p);

          // Does not affect OpenGL state
          void draw(ShaderProgramPtr p, std::vector<TexturePtr> textures);

          // Does not affect OpenGL state
          void set_tex(TexturePtr t);

          int width;
          int height;

          ShaderProgramPtr program;

        private:
          void populate_quad_buf_();

          TexturePtr tex_;
          VertexArrayObjectPtr vao_;

          VertexBuffer buf_;
          VertexBuffer texture_coord_buf_;

          MatrixX2f vertices_;
          MatrixX2f texture_coords_;

      };

    } // namespace geometry
  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_GEOMETRY_SCREEN_QUAD_H */
