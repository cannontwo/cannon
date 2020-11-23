#ifndef CANNON_GRAPHICS_VERTEX_COLOR_TEX_BUFFER_H
#define CANNON_GRAPHICS_VERTEX_COLOR_TEX_BUFFER_H 

#include <glad/glad.h>
#include <Eigen/Dense>

#include <cannon/graphics/vertex_array_object.hpp>
#include <cannon/graphics/vertex_buffer.hpp>

using namespace Eigen;

namespace cannon {
  namespace graphics {

    // TODO Remove entirely, this is deprecated and not certified by unit tests
    class VertexColorTexBuffer {
      public:
        VertexColorTexBuffer() = delete;

        VertexColorTexBuffer(std::shared_ptr<VertexArrayObject> vao) : vao_(vao) {
          vao_->bind();
          glGenBuffers(1, &gl_vertex_buffer_object_);

          gl_vertex_attribute_num_ = vao_->get_next_vertex_attribute_num();
          gl_color_attribute_num_ = vao_->get_next_vertex_attribute_num();
          gl_tex_attribute_num_ = vao_->get_next_vertex_attribute_num();
        }

        ~VertexColorTexBuffer() {
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
        int gl_tex_attribute_num_;
        std::shared_ptr<VertexArrayObject> vao_;
    };

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_VERTEX_COLOR_TEX_BUFFER_H */
