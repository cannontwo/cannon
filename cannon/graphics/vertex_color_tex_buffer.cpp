#include <cannon/graphics/vertex_color_tex_buffer.hpp>

using namespace cannon::graphics;

void VertexColorTexBuffer::bind() {
  vao_.bind();
  glBindBuffer(GL_ARRAY_BUFFER, gl_vertex_buffer_object_);
}

void VertexColorTexBuffer::unbind() {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  vao_.unbind();
}

void VertexColorTexBuffer::buffer(MatrixXf vertices) {
  bind();

  assert(vertices.cols() == 8);

  // Eigen stores matrices in column-major format, so we transpose because
  // OpenGL expects row-major
  MatrixXf tmp_vertices(vertices.cols(), vertices.rows());
  tmp_vertices.noalias() = vertices.transpose().eval();

  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * tmp_vertices.size(), 
      tmp_vertices.data(), GL_STATIC_DRAW);

  set_vertex_attribute_pointer();
}

void VertexColorTexBuffer::set_vertex_attribute_pointer() {
  // Vertex position attribute
  glVertexAttribPointer(gl_vertex_attribute_num_, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(gl_vertex_attribute_num_);

  // Vertex color attribute
  glVertexAttribPointer(gl_color_attribute_num_, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3*sizeof(float)));
  glEnableVertexAttribArray(gl_color_attribute_num_);

  // Vertex texture coordinate attribute
  glVertexAttribPointer(gl_tex_attribute_num_, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6*sizeof(float)));
  glEnableVertexAttribArray(gl_tex_attribute_num_);
}
