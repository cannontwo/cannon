#ifndef CANNON_GRAPHICS_GEOMETRY_CUBE_H
#define CANNON_GRAPHICS_GEOMETRY_CUBE_H 

/*!
 * \file cannon/graphics/geometry/cube.hpp
 * \brief Class containing Cube class definition.
 */

#include <memory>
#include <string>

#include <cannon/graphics/geometry/drawable_geom.hpp>
#include <cannon/utils/class_forward.hpp>

namespace cannon {
  namespace graphics {

    CANNON_CLASS_FORWARD(VertexArrayObject);
    CANNON_CLASS_FORWARD(ShaderProgram);
    CANNON_CLASS_FORWARD(VertexBuffer);

    namespace geometry {

      class Cube : public DrawableGeom {
        public:

          // Does not affect OpenGL state
          Cube(const std::string& v_src="shaders/mvp_uniform_color.vert", const
              std::string& f_src="shaders/pass_color.frag");

          // Does not affect OpenGL state
          Cube (std::shared_ptr<ShaderProgram> p);

          Cube(Cube& c);

          Cube(Cube&& c) : vao_(c.vao_), buf_(std::move(c.buf_)),
          normal_buf_(std::move(c.normal_buf_)),
          vertices_(c.vertices_), normals_(c.normals_) {
            program = c.program; 

            name_ = c.name_;

            material_ = c.material_;
          }

          virtual ~Cube() override {}

          // Does not affect OpenGL state
          virtual void draw(const Matrix4f& view, const Matrix4f& perspective) const override;

          // Does not affect OpenGL state
          virtual void draw(ShaderProgramPtr p, const Matrix4f&
              view, const Matrix4f& perspective) const override;

        private:
          void populate_bufs_();

          VertexArrayObjectPtr vao_;
          VertexBufferPtr buf_;
          VertexBufferPtr normal_buf_;

          MatrixX3f vertices_;
          MatrixX3f normals_;
      };

    } // namespace geometry
  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_GEOMETRY_CUBE_H */
