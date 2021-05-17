#ifndef CANNON_GRAPHICS_GEOMETRY_AXES_HINT_H
#define CANNON_GRAPHICS_GEOMETRY_AXES_HINT_H 

#include <cannon/graphics/geometry/drawable_geom.hpp>
#include <cannon/graphics/vertex_buffer.hpp>

namespace cannon {
  namespace graphics {

    CANNON_CLASS_FORWARD(VertexArrayObject);

    namespace geometry {

      class AxesHint : public DrawableGeom {
        public:

          // Does not affect OpenGL state
          AxesHint();

          virtual ~AxesHint() {}

          // Does not affect OpenGL state
          virtual void draw(const Matrix4f& view, const Matrix4f& perspective) const override;

          // Does not affect OpenGL state
          virtual void draw(ShaderProgramPtr p, const Matrix4f&
              view, const Matrix4f& perspective) const override;

        private:
          void populate_bufs_x_();
          void populate_bufs_y_();
          void populate_bufs_z_();

          VertexArrayObjectPtr vao_x_;
          VertexArrayObjectPtr vao_y_;
          VertexArrayObjectPtr vao_z_;
          VertexBuffer buf_x_;
          VertexBuffer buf_y_;
          VertexBuffer buf_z_;

          MatrixX3f vertices_x_;
          MatrixX3f vertices_y_;
          MatrixX3f vertices_z_;
      };

    } // namespace geometry
  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_GEOMETRY_AXES_HINT_H */
