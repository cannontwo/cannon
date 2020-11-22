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

        // Does not change OpenGL state
        ElementBuffer(std::shared_ptr<VertexArrayObject> vao) : vao_(vao) {
          vao_->bind();
          glGenBuffers(1, &gl_element_buffer_object_);
          vao_->unbind();
        }

        // Sets GL_ELEMENT_ARRAY_BUFFER_BINDING to 0
        ~ElementBuffer() {
          glDeleteBuffers(1, &gl_element_buffer_object_);
        }

        // Sets GL_VERTEX_ARRAY_BINDING to vao_->gl_vertex_array_object_ and
        // GL_ELEMENT_ARRAY_BUFFER_BINDING to gl_element_buffer_object
        void bind() const;

        // Sets GL_VERTEX_ARRAY_BINDING to 0 and
        // GL_ELEMENT_ARRAY_BUFFER_BINDING to 0
        void unbind() const;

        // Does not change OpenGL state
        void buffer(MatrixX3u indices);

      private:
        unsigned int gl_element_buffer_object_;
        std::shared_ptr<VertexArrayObject> vao_;
    };

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_ELEMENT_BUFFER_H */
