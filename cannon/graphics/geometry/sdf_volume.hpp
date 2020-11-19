#ifndef CANNON_GRAPHICS_GEOMETRY_SDF_VOLUME_H
#define CANNON_GRAPHICS_GEOMETRY_SDF_VOLUME_H 

#include <cannon/graphics/geometry/drawable_geom.hpp>
#include <cannon/graphics/vertex_array_object.hpp>
#include <cannon/graphics/vertex_buffer.hpp>

namespace cannon {
  namespace graphics {
    namespace geometry {

      class SDFVolume : public DrawableGeom {
        public:
          SDFVolume(std::shared_ptr<ShaderProgram> p) : vao_(new VertexArrayObject), buf_(vao_),
          vertices_(36, 3) {

            program = p;
            populate_bufs_();
            name_ = std::string("SDF Volume");

            material_.ambient = {1.0, 1.0, 1.0, 1.0};
            material_.diffuse = {1.0, 1.0, 1.0, 1.0};
            material_.specular = {1.0, 1.0, 1.0, 1.0};
          }

          SDFVolume(SDFVolume& o) : vao_(new VertexArrayObject), buf_(vao_),
          vertices_(o.vertices_) {

            program = o.program;
            buf_.buffer(vertices_);

            name_ = o.name_;

            material_ = o.material_;

          }

          SDFVolume(SDFVolume&& o) : vao_(o.vao_), buf_(std::move(o.buf_)),
          vertices_(o.vertices_) {

            program = o.program;
            name_ = o.name_;

            material_ = o.material_;

          }

          virtual ~SDFVolume() override {}

          virtual void draw(const Matrix4f& view, const Matrix4f& perspective) const override;
          virtual void draw(std::shared_ptr<ShaderProgram> p, const Matrix4f&
              view, const Matrix4f& perspective) const override;

        private:
          void populate_bufs_();

          std::shared_ptr<VertexArrayObject> vao_;
          VertexBuffer buf_;

          MatrixX3f vertices_;

      };

    } // namespace geometry
  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_GEOMETRY_SDF_VOLUME_H */
