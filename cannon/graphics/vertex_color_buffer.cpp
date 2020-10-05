#include <cannon/graphics/vertex_color_buffer.hpp>

using namespace cannon::graphics;

void VertexColorBuffer::bind() {
  vao_.bind();
  glBindBuffer(GL_ARRAY_BUFFER, gl_vertex_buffer_object_);
}

void VertexColorBuffer::unbind() {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  vao_.unbind();
}

void VertexColorBuffer::buffer(MatrixXf vertices) {
  bind();

  assert(vertices.cols() == 6);

  // Eigen stores matrices in column-major format, so we transpose because
  // OpenGL expects row-major
  MatrixXf tmp_vertices(vertices.cols(), vertices.rows());
  tmp_vertices.noalias() = vertices.transpose().eval();

  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * tmp_vertices.size(), 
      tmp_vertices.data(), GL_STATIC_DRAW);

  set_vertex_attribute_pointer();
}

void VertexColorBuffer::set_vertex_attribute_pointer() {
  glVertexAttribPointer(gl_vertex_attribute_num_, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(gl_vertex_attribute_num_);

  glVertexAttribPointer(gl_color_attribute_num_, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3*sizeof(float)));
  glEnableVertexAttribArray(gl_color_attribute_num_);
}
