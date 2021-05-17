#ifndef CANNON_GRAPHICS_GEOMETRY_TEXTURED_CUBE_H
#define CANNON_GRAPHICS_GEOMETRY_TEXTURED_CUBE_H 

#include <string>

#include <cannon/graphics/geometry/drawable_geom.hpp>
#include <cannon/graphics/texture.hpp>
#include <cannon/graphics/vertex_buffer.hpp>
#include <cannon/graphics/element_buffer.hpp>
#include <cannon/utils/class_forward.hpp>

namespace cannon {
  namespace graphics {

    CANNON_CLASS_FORWARD(VertexArrayObject);
    CANNON_CLASS_FORWARD(ShaderProgram);

    namespace geometry {

      class TexturedCube : public DrawableGeom {
        public:
          TexturedCube() = delete;

          // Does not affect OpenGL state
          TexturedCube (ShaderProgramPtr p, const std::string&
              d_tex="assets/container2.png", const std::string&
              s_tex="assets/container2_specular.png");

          TexturedCube(TexturedCube& c);

          TexturedCube(TexturedCube&& c) : vao_(c.vao_),
          buf_(std::move(c.buf_)), normal_buf_(std::move(c.normal_buf_)),
          texture_coord_buf_(std::move(c.texture_coord_buf_)), diffuse_tex_(c.diffuse_tex_),
          specular_tex_(c.specular_tex_), vertices_(c.vertices_),
          normals_(c.normals_) {

            program = c.program;
            name_ = c.name_;
          
          }

          virtual ~TexturedCube() override {}

          // Does not affect OpenGL state
          virtual void draw(const Matrix4f& view, const Matrix4f& perspective) const override;

          // Does not affect OpenGL state
          virtual void draw(ShaderProgramPtr p, const Matrix4f&
              view, const Matrix4f& perspective) const override;

        private:
          void populate_bufs_();

          VertexArrayObjectPtr vao_;
          VertexBuffer buf_;
          VertexBuffer normal_buf_;
          VertexBuffer texture_coord_buf_;

          Texture diffuse_tex_;
          Texture specular_tex_;

          MatrixX3f vertices_;
          MatrixX3f normals_;
          MatrixX2f texture_coords_;
      };

    } // namespace geometry
  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_GEOMETRY_TEXTURED_CUBE_H */
