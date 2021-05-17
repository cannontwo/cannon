#ifndef CANNON_GRAPHICS_GEOMETRY_SDF_VOLUME_H
#define CANNON_GRAPHICS_GEOMETRY_SDF_VOLUME_H 

#include <cannon/graphics/geometry/drawable_geom.hpp>
#include <cannon/graphics/vertex_buffer.hpp>
#include <cannon/utils/class_forward.hpp>

namespace cannon {
  namespace graphics {

    CANNON_CLASS_FORWARD(VertexArrayObject);
    CANNON_CLASS_FORWARD(ShaderProgram);

    namespace geometry {

      class SDFVolume : public DrawableGeom {
        public:

          // Does not affect OpenGL state
          SDFVolume(ShaderProgramPtr p);

          SDFVolume(SDFVolume& o);

          SDFVolume(SDFVolume&& o) : vao_(o.vao_), buf_(std::move(o.buf_)),
          vertices_(o.vertices_) {

            program = o.program;
            name_ = o.name_;

            material_ = o.material_;

          }

          virtual ~SDFVolume() override {}

          // Does not affect OpenGL state
          virtual void draw(const Matrix4f& view, const Matrix4f& perspective) const override;

          // Does not affect OpenGL state
          virtual void draw(ShaderProgramPtr p, const Matrix4f& view, const
              Matrix4f& perspective) const override;

        private:
          void populate_bufs_();

          VertexArrayObjectPtr vao_;
          VertexBuffer buf_;

          MatrixX3f vertices_;

      };

    } // namespace geometry
  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_GEOMETRY_SDF_VOLUME_H */
