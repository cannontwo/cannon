#ifndef CANNON_GRAPHICS_VERTEX_BUFFER_H
#define CANNON_GRAPHICS_VERTEX_BUFFER_H 

#include <glad/glad.h>
#include <Eigen/Dense>

using namespace Eigen;

namespace cannon {
  namespace graphics {
    
    class VertexBuffer {
      public:
        VertexBuffer() {
          glGenBuffers(1, &vertex_buffer_object_);
        }

        ~VertexBuffer() {
          glDeleteBuffers(1, &vertex_buffer_object_);
        }

        void bind();
        void unbind();
        void buffer(MatrixX3f vertices);

      private:
        unsigned int vertex_buffer_object_;
    };

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_VERTEX_BUFFER_H */
