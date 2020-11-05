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
          TexturedCube(const std::string& v_src="shaders/mvp_normals_tex.vert",
              const std::string& f_src="shaders/material_light_tex.frag", const
              std::string& d_tex="assets/container2.png", const std::string&
              s_tex="assets/container2_specular.png") : vao_(new
                VertexArrayObject), buf_(vao_), normal_buf_(vao_),
            texture_coord_buf_(vao_), program_(new ShaderProgram),
            diffuse_tex_(d_tex, true, GL_TEXTURE0), 
            specular_tex_(s_tex, true, GL_TEXTURE1), 
            vertices_(36, 3), normals_(36, 3), texture_coords_(36, 2) {

            auto v = load_vertex_shader(v_src);
            auto f = load_fragment_shader(f_src);
            program_->attach_shader(v);
            program_->attach_shader(f);
            program_->link();
            
            populate_bufs_();

          }

          TexturedCube (std::shared_ptr<ShaderProgram> program, const
              std::string& d_tex="assets/container2.png", const std::string&
              s_tex="assets/container2_specular.png" ) : 
            vao_(new VertexArrayObject), buf_(vao_), normal_buf_(vao_),
            texture_coord_buf_(vao_), program_(program),
            diffuse_tex_(d_tex, false, GL_TEXTURE0), 
            specular_tex_(s_tex, false, GL_TEXTURE1), 
            vertices_(36, 3), normals_(36, 3), texture_coords_(36, 2) {

            populate_bufs_(); 

          }

          TexturedCube(TexturedCube& c) : vao_(new VertexArrayObject),
          buf_(vao_), normal_buf_(vao_), texture_coord_buf_(vao_), program_(c.program_),
          diffuse_tex_(c.diffuse_tex_), specular_tex_(c.specular_tex_),
          vertices_(c.vertices_), normals_(c.normals_), texture_coords_(c.texture_coords_) {

            buf_.buffer(vertices_);
            normal_buf_.buffer(normals_);
            texture_coord_buf_.buffer(texture_coords_);

          }

          TexturedCube(TexturedCube&& c) : vao_(c.vao_),
          buf_(std::move(c.buf_)), normal_buf_(std::move(c.normal_buf_)),
          texture_coord_buf_(std::move(c.texture_coord_buf_)),
          program_(c.program_), diffuse_tex_(c.diffuse_tex_),
          specular_tex_(c.specular_tex_), vertices_(c.vertices_),
          normals_(c.normals_) {}

          virtual ~TexturedCube() override {}

          virtual void draw(const Matrix4f& view, const Matrix4f& perspective) const override;

        private:
          void populate_bufs_();

          std::shared_ptr<VertexArrayObject> vao_;
          VertexBuffer buf_;
          VertexBuffer normal_buf_;
          VertexBuffer texture_coord_buf_;
          std::shared_ptr<ShaderProgram> program_;

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
