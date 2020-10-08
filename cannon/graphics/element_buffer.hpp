#ifndef CANNON_GRAPHICS_ELEMENT_BUFFER_H
#define CANNON_GRAPHICS_ELEMENT_BUFFER_H 

#include <glad/glad.h>
#include <Eigen/Dense>

#include <cannon/graphics/vertex_array_object.hpp>

using namespace Eigen;

namespace cannon {
  namespace graphics {
    
    using MatrixX3u = Matrix<unsigned int, Dynamic, 3>;
    using Matrix3Xu = Matrix<unsigned int, 3, Dynamic>;

    class ElementBuffer {
      public:
        ElementBuffer() = delete;

        ElementBuffer(std::shared_ptr<VertexArrayObject> vao) : vao_(vao) {
          vao_->bind();
          glGenBuffers(1, &gl_element_buffer_object_);
        }

        ~ElementBuffer() {
          glDeleteBuffers(1, &gl_element_buffer_object_);
        }

        void bind();
        void unbind();
        void buffer(MatrixX3u indices);

      private:
        unsigned int gl_element_buffer_object_;
        std::shared_ptr<VertexArrayObject> vao_;
    };

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_ELEMENT_BUFFER_H */
