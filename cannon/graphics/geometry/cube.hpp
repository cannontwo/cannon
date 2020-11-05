#ifndef CANNON_GRAPHICS_GEOMETRY_CUBE_H
#define CANNON_GRAPHICS_GEOMETRY_CUBE_H 

#include <memory>
#include <string>

#include <cannon/graphics/geometry/drawable_geom.hpp>
#include <cannon/graphics/vertex_array_object.hpp>
#include <cannon/graphics/vertex_buffer.hpp>
#include <cannon/graphics/element_buffer.hpp>
#include <cannon/graphics/vertex_shader.hpp>
#include <cannon/graphics/fragment_shader.hpp>
#include <cannon/graphics/shader_program.hpp>

namespace cannon {
  namespace graphics {
    namespace geometry {

      class Cube : public DrawableGeom {
        public:
          Cube(const std::string& v_src="shaders/mvp_uniform_color.vert", const
              std::string& f_src="shaders/pass_color.frag") : 
            vao_(new VertexArrayObject), buf_(vao_), normal_buf_(vao_), 
            program_(new ShaderProgram), vertices_(36, 3), normals_(36, 3) {

            auto v = load_vertex_shader(v_src);
            auto f = load_fragment_shader(f_src);
            program_->attach_shader(v);
            program_->attach_shader(f);
            program_->link();
            
            populate_bufs_();
          }

          Cube (std::shared_ptr<ShaderProgram> program) : vao_(new
              VertexArrayObject), buf_(vao_), normal_buf_(vao_), program_(program),
          vertices_(36, 3), normals_(36, 3) {
            populate_bufs_(); 
          }

          Cube(Cube& c) : vao_(new VertexArrayObject), buf_(vao_), normal_buf_(vao_),
          program_(c.program_), vertices_(c.vertices_), normals_(c.normals_) {
            buf_.buffer(vertices_);
            normal_buf_.buffer(normals_);
          }

          Cube(Cube&& c) : vao_(c.vao_), buf_(std::move(c.buf_)),
          normal_buf_(std::move(c.normal_buf_)), program_(c.program_),
          vertices_(c.vertices_), normals_(c.normals_) {}

          virtual ~Cube() override {}

          virtual void draw(const Matrix4f& view, const Matrix4f& perspective) const override;

        private:
          void populate_bufs_();

          std::shared_ptr<VertexArrayObject> vao_;
          VertexBuffer buf_;
          VertexBuffer normal_buf_;
          std::shared_ptr<ShaderProgram> program_;

          MatrixX3f vertices_;
          MatrixX3f normals_;
      };

    } // namespace geometry
  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_GEOMETRY_CUBE_H */