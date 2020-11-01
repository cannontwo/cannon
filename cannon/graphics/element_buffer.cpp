#include <cannon/graphics/element_buffer.hpp>

using namespace cannon::graphics;

void ElementBuffer::bind() const {
  vao_->bind();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_element_buffer_object_);
}

void ElementBuffer::unbind() const {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  vao_->unbind();
}

void ElementBuffer::buffer(MatrixX3u indices) {
  bind();

  // Eigen stores matrices in column-major format, so we transpose because
  // OpenGL expects row-major
  Matrix3Xu tmp_indices;
  tmp_indices.noalias() = indices.transpose().eval();
  
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * tmp_indices.size(), 
      tmp_indices.data(), GL_STATIC_DRAW);
}
