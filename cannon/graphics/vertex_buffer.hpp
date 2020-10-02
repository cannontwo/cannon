#ifndef CANNON_GRAPHICS_VERTEX_BUFFER_H
#define CANNON_GRAPHICS_VERTEX_BUFFER_H 

#include <glad/glad.h>
#include <Eigen/Dense>

#include <cannon/graphics/vertex_array_object.hpp>

using namespace Eigen;

namespace cannon {
  namespace graphics {
    
    class VertexBuffer {
      public:
        VertexBuffer() = delete;

        VertexBuffer(VertexArrayObject& vao) : vao_(vao) {
          vao_.bind();
          glGenBuffers(1, &gl_vertex_buffer_object_);
        }

        ~VertexBuffer() {
          glDeleteBuffers(1, &gl_vertex_buffer_object_);
        }

        void bind();
        void unbind();
        void buffer(MatrixX3f vertices);

      private:
        unsigned int gl_vertex_buffer_object_;
        VertexArrayObject& vao_;
    };

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_VERTEX_BUFFER_H */
