#ifndef CANNON_GRAPHICS_GEOMETRY_PLANE_H
#define CANNON_GRAPHICS_GEOMETRY_PLANE_H 

#include <cannon/graphics/geometry/drawable_geom.hpp>

#include <cannon/graphics/vertex_buffer.hpp>
#include <cannon/utils/class_forward.hpp>

namespace cannon {
  namespace graphics {

    CANNON_CLASS_FORWARD(VertexArrayObject);

    namespace geometry {

      class Plane : public DrawableGeom {
        public:

          // Does not affect OpenGL state
          Plane(std::shared_ptr<ShaderProgram> p);

          Plane(Plane& o);

          Plane(Plane&& o) : vao_(o.vao_), buf_(std::move(o.buf_)),
          normal_buf_(std::move(o.normal_buf_)),
          vertices_(o.vertices_), normals_(o.normals_) {
            program = o.program; 

            name_ = o.name_;

            material_ = o.material_;
          }

          virtual ~Plane() override {}

          // Does not affect OpenGL state
          virtual void draw(const Matrix4f& view, const Matrix4f& perspective) const override;

          // Does not affect OpenGL state
          virtual void draw(std::shared_ptr<ShaderProgram> p, const Matrix4f&
              view, const Matrix4f& perspective) const override;

        private:
          void populate_bufs_();

          VertexArrayObjectPtr vao_;
          VertexBuffer buf_;
          VertexBuffer normal_buf_;

          MatrixX3f vertices_;
          MatrixX3f normals_;

      };

    } // namespace geometry
  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_GEOMETRY_PLANE_H */
