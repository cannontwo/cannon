#ifndef CANNON_GRAPHICS_VERTEX_COLOR_BUFFER_H
#define CANNON_GRAPHICS_VERTEX_COLOR_BUFFER_H 

#include <glad/glad.h>
#include <Eigen/Dense>

#include <cannon/graphics/vertex_array_object.hpp>
#include <cannon/graphics/vertex_buffer.hpp>

using namespace Eigen;

namespace cannon {
  namespace graphics {
    class VertexColorBuffer {
      public:
        VertexColorBuffer() = delete;

        VertexColorBuffer(VertexArrayObject& vao) : vao_(vao) {
          vao_.bind();
          glGenBuffers(1, &gl_vertex_buffer_object_);

          gl_vertex_attribute_num_ = get_next_vertex_attribute_num();
          gl_color_attribute_num_ = get_next_vertex_attribute_num();
        }

        ~VertexColorBuffer() {
          glDeleteBuffers(1, &gl_vertex_buffer_object_);
        }

        void bind();
        void unbind();
        void buffer(MatrixXf vertices);

      private:
        void set_vertex_attribute_pointer();

        unsigned int gl_vertex_buffer_object_;
        int gl_vertex_attribute_num_;
        int gl_color_attribute_num_;
        VertexArrayObject& vao_;
    };

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_VERTEX_COLOR_BUFFER_H */
