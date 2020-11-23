#ifndef CANNON_GRAPHICS_VERTEX_BUFFER_H
#define CANNON_GRAPHICS_VERTEX_BUFFER_H 

#include <glad/glad.h>
#include <Eigen/Dense>
#include <iostream>

#include <cannon/graphics/vertex_array_object.hpp>
#include <cannon/log/registry.hpp>

using namespace Eigen;
using namespace cannon::log;

namespace cannon {
  namespace graphics {

    class VertexBuffer {
      public:
        VertexBuffer() : vao_(nullptr) {}

        VertexBuffer(std::shared_ptr<VertexArrayObject> vao) : vao_(vao) {
          vao_->bind();
          glGenBuffers(1, &gl_vertex_buffer_object_);

          gl_vertex_attribute_num_ = vao_->get_next_vertex_attribute_num();
          vao_->unbind();
        }

        VertexBuffer(VertexBuffer& buf) = delete;

        VertexBuffer(VertexBuffer&& buf) :
          gl_vertex_buffer_object_(buf.gl_vertex_buffer_object_),
          gl_vertex_attribute_num_(buf.gl_vertex_attribute_num_),
          vao_(buf.vao_) {
            buf.gl_vertex_buffer_object_ = -1;
          }

        ~VertexBuffer() {
          bind();
          glDeleteBuffers(1, &gl_vertex_buffer_object_);
          unbind();
        }

        // Does not affect OpenGL State
        void init(std::shared_ptr<VertexArrayObject> vao);

        // Sets GL_VERTEX_ARRAY_BINDING to vao_->gl_vertex_array_object_ and
        // GL_ARRAY_BUFFER_BINDING to gl_vertex_buffer_object
        void bind() const;

        // Sets GL_VERTEX_ARRAY_BINDING to 0 and GL_ARRAY_BUFFER_BINDING to 0
        void unbind() const;

        // Does not affect OpenGL state
        void buffer(MatrixX2f vertices);

        // Does not affect OpenGL state
        void buffer(MatrixX3f vertices);

        // Does not affect OpenGL state
        void buffer(MatrixX4f vertices);

        // Does not affect OpenGL state
        void replace(MatrixX2f vertices);

        // Does not affect OpenGL state
        void replace(MatrixX3f vertices);

        // Does not affect OpenGL state
        void replace(MatrixX4f vertices);

        friend std::ostream& operator<<(std::ostream&, const VertexBuffer&);

      private:
        void set_vertex_attribute_pointer(int dim);

        unsigned int gl_vertex_buffer_object_;
        int gl_vertex_attribute_num_;
        std::shared_ptr<VertexArrayObject> vao_;
    };

    std::ostream& operator<<(std::ostream&, const VertexBuffer&);

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_VERTEX_BUFFER_H */
