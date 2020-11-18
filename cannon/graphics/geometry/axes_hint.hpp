#ifndef CANNON_GRAPHICS_GEOMETRY_AXES_HINT_H
#define CANNON_GRAPHICS_GEOMETRY_AXES_HINT_H 

#include <cannon/graphics/geometry/drawable_geom.hpp>
#include <cannon/graphics/vertex_buffer.hpp>
#include <cannon/graphics/vertex_array_object.hpp>

namespace cannon {
  namespace graphics {
    namespace geometry {

      class AxesHint : public DrawableGeom {
        public:

          AxesHint() : vao_x_(new VertexArrayObject), vao_y_(new
              VertexArrayObject), vao_z_(new VertexArrayObject),
          buf_x_(vao_x_), buf_y_(vao_y_), buf_z_(vao_z_), vertices_x_(2, 3),
          vertices_y_(2, 3), vertices_z_(2, 3) {

            populate_bufs_x_();
            populate_bufs_y_();
            populate_bufs_z_();

            program = std::make_shared<ShaderProgram>();
            program->attach_vertex_shader("shaders/pass_pos.vert");
            program->attach_fragment_shader("shaders/axes_hint.frag");
            program->link();
          }

          virtual ~AxesHint() {}

          virtual void draw(const Matrix4f& view, const Matrix4f& perspective) const override;
          virtual void draw(std::shared_ptr<ShaderProgram> p, const Matrix4f&
              view, const Matrix4f& perspective) const override;

        private:
          void populate_bufs_x_();
          void populate_bufs_y_();
          void populate_bufs_z_();

          std::shared_ptr<VertexArrayObject> vao_x_;
          std::shared_ptr<VertexArrayObject> vao_y_;
          std::shared_ptr<VertexArrayObject> vao_z_;
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
