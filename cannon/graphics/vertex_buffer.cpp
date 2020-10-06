#include <cannon/graphics/vertex_buffer.hpp>

using namespace cannon::graphics;

void VertexBuffer::bind() {
  vao_.bind();
  glBindBuffer(GL_ARRAY_BUFFER, gl_vertex_buffer_object_);
}

void VertexBuffer::unbind() {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  vao_.unbind();
}

void VertexBuffer::buffer(MatrixX3f vertices) {
  bind();

  // Eigen stores matrices in column-major format, so we transpose because
  // OpenGL expects row-major
  Matrix3Xf tmp_vertices;
  tmp_vertices.noalias() = vertices.transpose().eval();

  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * tmp_vertices.size(), 
      tmp_vertices.data(), GL_STATIC_DRAW);

  set_vertex_attribute_pointer(3);
}

void VertexBuffer::buffer(MatrixX2f vertices) {
  bind();

  // Eigen stores matrices in column-major format, so we transpose because
  // OpenGL expects row-major
  Matrix2Xf tmp_vertices;
  tmp_vertices.noalias() = vertices.transpose().eval();

  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * tmp_vertices.size(), 
      tmp_vertices.data(), GL_STATIC_DRAW);

  set_vertex_attribute_pointer(2);
}

void VertexBuffer::set_vertex_attribute_pointer(int dim) {
  glVertexAttribPointer(gl_vertex_attribute_num_, dim, GL_FLOAT, GL_FALSE, dim * sizeof(float), (void *)0);
  glEnableVertexAttribArray(gl_vertex_attribute_num_);
}

// Free Functions
int cannon::graphics::get_next_vertex_attribute_num() {
  int max;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max);

  if (VERTEX_ATTRIBUTES_DECLARED >= max)
    throw std::runtime_error("Too many vertex attributes declared");

  VERTEX_ATTRIBUTES_DECLARED = VERTEX_ATTRIBUTES_DECLARED + 1;
  return VERTEX_ATTRIBUTES_DECLARED - 1;
}

