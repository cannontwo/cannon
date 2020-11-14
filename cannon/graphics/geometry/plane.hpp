#ifndef CANNON_GRAPHICS_GEOMETRY_PLANE_H
#define CANNON_GRAPHICS_GEOMETRY_PLANE_H 

#include <cannon/graphics/geometry/drawable_geom.hpp>

#include <cannon/graphics/vertex_array_object.hpp>
#include <cannon/graphics/vertex_buffer.hpp>

namespace cannon {
  namespace graphics {
    namespace geometry {

      class Plane : public DrawableGeom {
        public:
          Plane(std::shared_ptr<ShaderProgram> p) : vao_(new
              VertexArrayObject), buf_(vao_), normal_buf_(vao_),
          vertices_(6, 3), normals_(6, 3) {

            program = p;

            populate_bufs_(); 

            name_ = std::string("Plane");

            material_.ambient = {1.0, 1.0, 1.0, 1.0};
            material_.diffuse = {1.0, 1.0, 1.0, 1.0};
            material_.specular = {1.0, 1.0, 1.0, 1.0};
          }

          Plane(Plane& o) : vao_(new VertexArrayObject), buf_(vao_), normal_buf_(vao_),
          vertices_(o.vertices_), normals_(o.normals_) {
            program = o.program;
            buf_.buffer(vertices_);
            normal_buf_.buffer(normals_);

            name_ = o.name_;

            material_ = o.material_;
          }

          Plane(Plane&& o) : vao_(o.vao_), buf_(std::move(o.buf_)),
          normal_buf_(std::move(o.normal_buf_)),
          vertices_(o.vertices_), normals_(o.normals_) {
            program = o.program; 

            name_ = o.name_;

            material_ = o.material_;
          }

          virtual ~Plane() override {}

          virtual void draw(const Matrix4f& view, const Matrix4f& perspective) const override;
          virtual void draw(std::shared_ptr<ShaderProgram> p, const Matrix4f&
              view, const Matrix4f& perspective) const override;

        private:
          void populate_bufs_();

          std::shared_ptr<VertexArrayObject> vao_;
          VertexBuffer buf_;
          VertexBuffer normal_buf_;

          MatrixX3f vertices_;
          MatrixX3f normals_;

      };

    } // namespace geometry
  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_GEOMETRY_PLANE_H */
