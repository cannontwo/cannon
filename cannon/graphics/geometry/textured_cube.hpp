#ifndef CANNON_GRAPHICS_GEOMETRY_TEXTURED_CUBE_H
#define CANNON_GRAPHICS_GEOMETRY_TEXTURED_CUBE_H 

#include <memory>
#include <string>

#include <cannon/graphics/geometry/drawable_geom.hpp>
#include <cannon/graphics/vertex_array_object.hpp>
#include <cannon/graphics/vertex_buffer.hpp>
#include <cannon/graphics/element_buffer.hpp>
#include <cannon/graphics/vertex_shader.hpp>
#include <cannon/graphics/fragment_shader.hpp>
#include <cannon/graphics/shader_program.hpp>
#include <cannon/graphics/texture.hpp>

namespace cannon {
  namespace graphics {
    namespace geometry {

      class TexturedCube : public DrawableGeom {
        public:
          TexturedCube() = delete;

          // Does not affect OpenGL state
          TexturedCube (std::shared_ptr<ShaderProgram> p, const
              std::string& d_tex="assets/container2.png", const std::string&
              s_tex="assets/container2_specular.png" ) : 
            vao_(new VertexArrayObject), buf_(vao_), normal_buf_(vao_),
            texture_coord_buf_(vao_),
            diffuse_tex_(d_tex, false, GL_TEXTURE0), 
            specular_tex_(s_tex, false, GL_TEXTURE1), 
            vertices_(36, 3), normals_(36, 3), texture_coords_(36, 2) {

            program = p;

            populate_bufs_(); 

            name_ = std::string("Textured Cube");
          }

          TexturedCube(TexturedCube& c) : vao_(new VertexArrayObject),
          buf_(vao_), normal_buf_(vao_), texture_coord_buf_(vao_),
          diffuse_tex_(c.diffuse_tex_), specular_tex_(c.specular_tex_),
          vertices_(c.vertices_), normals_(c.normals_), texture_coords_(c.texture_coords_) {

            program = c.program;

            buf_.buffer(vertices_);
            normal_buf_.buffer(normals_);
            texture_coord_buf_.buffer(texture_coords_);

            name_ = c.name_;

          }

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
          virtual void draw(std::shared_ptr<ShaderProgram> p, const Matrix4f&
              view, const Matrix4f& perspective) const override;

        private:
          void populate_bufs_();

          std::shared_ptr<VertexArrayObject> vao_;
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
