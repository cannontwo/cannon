#include <cannon/graphics/vertex_buffer.hpp>

using namespace cannon::graphics;

void VertexBuffer::bind() {
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
}

void VertexBuffer::unbind() {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::buffer(MatrixX3f vertices) {
  bind();
  // Eigen stores matrices in column-major format, so we transpose because
  // OpenGL expects row-major
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), 
      vertices.transpose().data(), GL_STATIC_DRAW);
  unbind();
}
